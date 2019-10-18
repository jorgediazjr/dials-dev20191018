#include <cmath>
//#include <math.h>
#include <iostream>
#include <iomanip>

#include <scitbx/array_family/shared_algebra.h>
#include <scitbx/array_family/versa_algebra.h>

#include <mmtbx/tls/tls.h>
#include <mmtbx/tls/utils.h>

// define rint for Windows
#if defined(_WIN32)
double rint(double x)
{
  //middle value point test
  if (ceil(x+0.5) == floor(x+0.5))
  {
    int a = (int)ceil(x);
    if (a%2 == 0) { return ceil(x); }
    else { return floor(x); }
  }
  else { return floor(x+0.5); }
}
#endif

namespace mmtbx { namespace tls { namespace utils {

inline TLSComponent operator|(TLSComponent a, TLSComponent b)
{
  return static_cast<TLSComponent>(static_cast<int>(a) | static_cast<int>(b));
}

// Replace all instances of a substring in a string with another string
std::string find_and_replace(std::string source, std::string const& find, std::string const& replace)
{
  for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
  {
    source.replace(i, find.length(), replace);
    i += replace.length();
  }
  return source;
}

vecArrNd uij_eigenvalues(const symArrNd& uijs)
{
  vecArrNd uij_eig_vals(af::flex_grid<>(uijs.accessor().all()));
  for (int i = 0; i < uijs.size(); i++)
  {
    sym const *u = &uijs[i];
    dec::Eig es(*u);
    dblArr1d ev = es.values();
    memcpy(&uij_eig_vals[i], &ev[0], sizeof(double)*3);
  }

  return uij_eig_vals;
}

//! Check that arrays are compatible for calculating uijs
void validateSitesAndOrigins(const vecArrNd& sites_carts, const vecArr1d& origins)
{
  // Check input array is correct dimension
  size_t nd = sites_carts.accessor().nd();
  if (nd != 2)
  {
    throw std::invalid_argument( "sites_carts must be 2-dimensional array of size (n_dst, n_atm)" );
  }
  // Check that length of origins is same as first dimension of sites_carts
  size_t n_dst = sites_carts.accessor().all()[0];
  if (origins.size()!=n_dst)
  {
    throw std::invalid_argument( "Mismatch between the size of origins and first dimension of sites_carts" );
  }
}

//
// TLS MATRICES
//

double TLSMatrices::tol = 1e-6;
double TLSMatrices::rnd = 1e6;    // Precision of log10(val)

//! Constructor from nothing
TLSMatrices::TLSMatrices()
{
  reset();
}

//! Constructor from separate matrices
TLSMatrices::TLSMatrices(const sym &T, const sym &L, const mat &S)
  : T(T), L(L), S(S)
{
  round();
}

//! Constructor from another TLSMatrices object
TLSMatrices::TLSMatrices(const TLSMatrices &other) {
  T = other.T;
  L = other.L;
  S = other.S;
  // Do not need to round as populated from other object
}

//! Constructor from array of 21 values
TLSMatrices::TLSMatrices(const dblArr1d &values)
{
  if (values.size()!=21)
  {
    throw std::invalid_argument( "Input values must have length 21" );
  }
  T = sym(&values[0]);
  L = sym(&values[6]);
  S = mat(&values[12]);
  round();
}

//! Get number of decimal places for rounding values
int TLSMatrices::getPrecision()
{
  return log10(rnd);
}

//! Set number of decimal places for rounding values
void TLSMatrices::setPrecision(int decimals)
{
  rnd = pow(10.0, (double)decimals);
}

//! Get default tolerance for determining validity of matrices
double TLSMatrices::getTolerance()
{
  return tol;
}

//! Set default tolerance for determining validity of matrices
void TLSMatrices::setTolerance(double tolerance)
{
  if (tolerance <= 0.0)
  {
    throw std::invalid_argument( "tolerance must be greater than 0.0" );
  }
  tol = tolerance;
}

//! Override + operator for adding two classes together.
TLSMatrices& TLSMatrices::operator+(const TLSMatrices &other) const
{
  TLSMatrices& c = *copy();
  c.add(other);
  return c;
}

//! Override * operator for multiplying by a scalar
TLSMatrices& TLSMatrices::operator*(double scalar) const
{
  TLSMatrices& c = *copy();
  c.multiply(scalar);
  return c;
}

//! Add another TLSMatrices object to this one, matrix by matrix.
void TLSMatrices::add(const TLSMatrices &other)
{
  T += other.T;
  L += other.L;
  S += other.S;
  round();
}

//! Test if the matrix values of any of the selected components are nonzero
bool TLSMatrices::any(const std::string & component_string, double tolerance)
{
  sanitiseTolerance(&tolerance);
  TLSComponent components = stringToComponents(component_string);
  return anyByInt(components, tolerance);
}

//! Create a copy of the class
TLSMatrices* TLSMatrices::copy() const
{
  return new TLSMatrices(*this);
}

//! Return a class that determines the physical motions associated with the TLS matrices
dec::decompose_tls_matrices TLSMatrices::decompose(double tolerance) const
{
  sanitiseTolerance(&tolerance);
  dec::decompose_tls_matrices decomposition(T, L, S, tolerance);
  return decomposition;
}

//! Extract matrix values as an array, given an enum 0-7 which selects combinations of T(1), L(2) & S(4) by bitwise operations.
dblArr1d TLSMatrices::getValuesByInt(const TLSComponent &components, bool include_szz) const
{
  // Output array
  dblArr1d values;
  // Output T values?
  if (components & TLSTranslation)
  {
    values.reserve(values.size() + T.size());
    std::copy(T.begin(), T.end(), std::back_inserter(values));
  }
  // Output L values?
  if (components & TLSLibration)
  {
    values.reserve(values.size() + L.size());
    std::copy(L.begin(), L.end(), std::back_inserter(values));
  }
  // Output S values?
  if (components & TLSScrew)
  {
    if (include_szz)
    {
      // Copy all elements of the S-matrix
      values.reserve(values.size() + S.size());
      std::copy(S.begin(), S.end(), std::back_inserter(values));
    } else {
      // Copy first 8 elements of the S-matrix
      values.reserve(values.size() + S.size()-1);
      std::copy(S.begin(), S.end()-1, std::back_inserter(values));
    }
  }
  return values;
}

//! Extract matrix values as an array, given a selection string containing combinations of T,L or S.
dblArr1d TLSMatrices::getValuesByString(const std::string & component_string, bool include_szz) const
{
  TLSComponent components = stringToComponents(component_string);
  return getValuesByInt(components, include_szz=include_szz);
}

//! Determine whether the TLS matrices are physically valid by decomposition
bool TLSMatrices::isValid(double tolerance) const
{
  dec::decompose_tls_matrices decomp = decompose(tolerance);
  return decomp.is_valid();
}

//! Scale all matrix values by a scalar
void TLSMatrices::multiply(double scalar)
{
  if (scalar < 0.0)
  {
    throw std::invalid_argument( "Multiplier must be positive" );
  }
  scale(scalar);
  round();
}

//! Scale the TLS matrices so that the generated Uij are on a particular scale.
/*!
 * On failure, return negative value
 */
double TLSMatrices::normalise(const vecArr1d &sites_cart, const vec &origin, double target, double tolerance)
{
  sanitiseTolerance(&tolerance);
  // Target musr be positive
  if ( target <= 0.0 )
  {
    throw std::invalid_argument( "target must be positive" );
  }
  // Error if TLS Matrices are invalid
  if (!isValid(tolerance))
  {
    throw std::runtime_error( "TLS Matrices are invalid -- cannot normalise matrices" );
  }
  // Eventual multiplier to be applied to this object
  double mult = 0;
  // Extract the uijs for the provided coordinates
  symArr1d uaniso = uijs(sites_cart, origin);
  // Sum of the Uij eigenvalues
  double sum = 0;
  for (int i = 0; i < uaniso.size(); i++)
  {
    // Calculate the eigenvalues of this Uij
    sym *u = &uaniso[i];
    dec::Eig es(*u);
    dblArr1d ev = es.values();
    // Calculate sum of the eigenvalues
    for (int j = 0; j < ev.size(); j++)
    {
      if (ev[j] > 0.0)
      {
        sum += ev[j];
      }
    }
  }
  // Calculate the average eigenvalue
  double mean = sum / (3.0 * (double)uaniso.size());
  // If the average eigenvalue is negligible or negative
  // we cannot normalise (dividing by approx zero)
  if (mean < tolerance)
  {
    return -1;
  }
  // The multiplier to be applied to the matrices is the
  // ratio of the current mean to the target mean
  mult = target / mean ;
  // Apply multiplier and set Szz value
  multiply(mult);
  setSzzValueFromSxxAndSyy();
  // Return inverse of multiplier to be applied to amplitudes
  return 1.0 / mult;
}

//! Determine the number of parameters (total or free)
int TLSMatrices::paramCount(bool free, bool non_zero)
{
  int remove = free;
  if (!non_zero)
  {
    // (condition ? value-if-true : value-if-false)
    return T.size() + L.size() + S.size() - remove;
  }
  int result = 0;
  result += anyByInt(TLSTranslation) * T.size();
  result += anyByInt(TLSLibration)   * L.size();
  result += anyByInt(TLSScrew)       * (S.size() - remove);
  return result;
}

//! Set all matrix elements to 0
void TLSMatrices::reset()
{
  //scale(0.0);
  memset(&T[0], '\0', sizeof(double) * T.size());
  memset(&L[0], '\0', sizeof(double) * L.size());
  memset(&S[0], '\0', sizeof(double) * S.size());
}

//! Set matrices values from an array and a matrix selection enum (0-7)
void TLSMatrices::setValuesByInt(const dblArr1d &values, const TLSComponent &components, bool include_szz)
{
  // Validate length of input array
  int subtract_szz = (!include_szz);
  int req_length = ( (bool)(components & TLSTranslation) * T.size() ) +
                   ( (bool)(components & TLSLibration  ) * L.size() ) +
                   ( (bool)(components & TLSScrew      ) * (S.size() - subtract_szz) );
  if (req_length != values.size())
  {
    throw std::invalid_argument( "Mismatch between the length of the selected matrices and the length of the input array" );
  }
  // Copy chunks to matrices are required
  size_t current = 0;
  if (components & TLSTranslation)
  {
    memcpy(&T[0], &values[current], sizeof(double) * T.size());
    current += T.size();
  }
  if (components & TLSLibration)
  {
    memcpy(&L[0], &values[current], sizeof(double) * L.size());
    current += L.size();
  }
  if (components & TLSScrew)
  {
    if (include_szz)
    {
      memcpy(&S[0], &values[current], sizeof(double) * S.size());
      current += S.size();
    } else {
      memcpy(&S[0], &values[current], sizeof(double) * (S.size()-1));
      current += (S.size()-1);
      round();
      setSzzValueFromSxxAndSyy();
    }
  }
  round();
  if (current!=values.size())
  {
    throw std::runtime_error( "Mismatch between the current index and the length of the input array" );
  }
}

//! Set matrices vaulues from an array and a matrix selection string (containing combinations of T, L or S)
void TLSMatrices::setValuesByString(const dblArr1d &values, const std::string &component_string, bool include_szz)
{
  TLSComponent components = stringToComponents(component_string);
  setValuesByInt(values, components, include_szz=include_szz);
}

//! Generates summary string for the class
std::string TLSMatrices::summary()
{
  std::ostringstream buff;
  buff << "> TLS Matrix Parameters";
  std::string spacer = "         ";
  buff << std::endl << std::endl << "    T:   ";
  buff << find_and_replace(matrix_to_string(T), "\n", "\n"+spacer);
  buff << std::endl << std::endl << "    L:   ";
  buff << find_and_replace(matrix_to_string(L), "\n", "\n"+spacer);
  buff << std::endl << std::endl << "    S:   ";
  buff << find_and_replace(matrix_to_string(S), "\n", "\n"+spacer);
  return buff.str();
}

//! Convert TLS matrices to Uijs from coordinates and an origin
symArr1d TLSMatrices::uijs(const vecArr1d &sites_cart, const vec &origin) const
{
  mmtbx::tls::tlso<double> tls_params = mmtbx::tls::tlso<double>(T, L, S, origin);
  symArr1d uaniso;
  uaniso = uaniso_from_tls_one_group(tls_params, sites_cart, false);
  return uaniso;
}

//! Returns T-matrix
sym TLSMatrices::getT() { return T; }

//! Returns L-matrix
sym TLSMatrices::getL() { return L; }

//! Returns S-matrix
mat TLSMatrices::getS() { return S; }

//! Test if the matrix values of any of the selected components are nonzero (using enum 0-7)
bool TLSMatrices::anyByInt(const TLSComponent &components, double tolerance)
{
  sanitiseTolerance(&tolerance);
  dblArr1d values = getValuesByInt(components);
  for (int i = 0; i < values.size(); i++)
  {
    if (fabs(values[i]) > tolerance)
    {
      return true;
    }
  }
  return false;
}

//! Format a sym_mat3 matrix neatly to buffer
std::string TLSMatrices::matrix_to_string(const sym &M)
{
  std::ostringstream buff;
  int dec = (int)log10(rnd);
  int wth = dec + 5;
  buff << std::setprecision(dec)
       << std::showpos
       << std::setw(wth) << M[0] << "  "
       << std::setw(wth) << M[3] << "  "
       << std::setw(wth) << M[4] << std::endl
       << std::setw(wth) << "--" << "  "
       << std::setw(wth) << M[1] << "  "
       << std::setw(wth) << M[5] << std::endl
       << std::setw(wth) << "--" << "  "
       << std::setw(wth) << "--" << "  "
       << std::setw(wth) << M[2];
  return buff.str();
}

//! Format a mat3 matrix neatly to buffer
std::string TLSMatrices::matrix_to_string(const mat &M)
{
  std::ostringstream buff;
  int dec = (int)log10(rnd);
  int wth = dec + 5;
  buff << std::setprecision(dec)
       << std::showpos
       << std::setw(wth) << M[0] << "  "
       << std::setw(wth) << M[1] << "  "
       << std::setw(wth) << M[2] << std::endl
       << std::setw(wth) << M[3] << "  "
       << std::setw(wth) << M[4] << "  "
       << std::setw(wth) << M[5] << std::endl
       << std::setw(wth) << M[6] << "  "
       << std::setw(wth) << M[7] << "  "
       << std::setw(wth) << M[8] << std::endl;
  return buff.str();
}

//! Round matrix values to the set precision
void TLSMatrices::round()
{
  for (int i=0; i<6; i++)
  {
    T[i] = rint(rnd*T[i]) / rnd;
    L[i] = rint(rnd*L[i]) / rnd;
  }
  for (int i=0; i<9; i++)
  {
    S[i] = rint(rnd*S[i]) / rnd;
  }
}

//! Set a tolerance value to the default value if less than zero
void TLSMatrices::sanitiseTolerance(double *tolerance) const
{
  if (*tolerance < 0.0)
  {
    // Check that tolerance == -1
    if (*tolerance != -1)
    {
      throw std::invalid_argument( "Tolerance provided must either be positive or -1" );
    }
    // Set the pointer value to the internal default value
    *tolerance = tol;
  }
}

//! Scale all matrices by a scalar
void TLSMatrices::scale(double mult)
{
  scaleComponent(&T[0], T.size(), mult);
  scaleComponent(&L[0], L.size(), mult);
  scaleComponent(&S[0], S.size(), mult);
}

//! Scale a block of values by a scalar
void TLSMatrices::scaleComponent(double *comp, int size, double mult)
{
  for (int i = 0; i < size; i++)
  {
    comp[i] *= mult;
  }
}

//! Set the trace of the S-matrix by changing Szz
void TLSMatrices::setSzzValueFromSxxAndSyy(double target_trace)
{
  S[8] = target_trace - (S[0] + S[4]);
}

//! Convert a string to TLSComponents enum based on whether it contains the letters T, L or S
TLSComponent TLSMatrices::stringToComponents(const std::string &component_string) const
{
  if (component_string.size() == 0)
  {
    throw std::invalid_argument( "Empty string provided: '"+component_string+"'" );
  }
  TLSComponent total = TLSBlankString;
  size_t found;
  size_t count = 0;
  // Look for T in the component string
  found = component_string.find('T');
  if (found!=std::string::npos)
  {
    total = total | TLSTranslation;
    count++;
  }
  // Look for L in the component string
  found = component_string.find('L');
  if (found!=std::string::npos)
  {
    total = total | TLSLibration;
    count++;
  }
  // Look for S in the component string
  found = component_string.find('S');
  if (found!=std::string::npos)
  {
    total = total | TLSScrew;
    count++;
  }
  // Check that all letters have been used (no letters other than TLS)
  if (count != component_string.size())
  {
    throw std::invalid_argument( "Invalid letters in string (not T, L or S): '"+component_string+"'" );
  }
  return total;
}

//! Overload that allows left-multiplication of TLSMatrices/TLSAmplitudes
TLSMatrices operator*(const double &v, const TLSMatrices &x)
{
  TLSMatrices result = x * v;
  return result;
}

//
// TLS AMPLITUDES
//

double TLSAmplitudes::tol = 1e-6;
double TLSAmplitudes::rnd = 1e6;  // Precision of log10(val)

// Description strings
std::string TLSAmplitudes::short_description = "single amplitude per TLS group";
std::string TLSAmplitudes::description = "\n"
  "    TLS amplitude model:\n"
  "        One amplitude (a) per TLS model.\n"
  "        All TLS matrices are coupled together.\n"
  "        \n"
  "        T -> a * T\n"
  "        L -> a * L\n"
  "        S -> a * S\n";

//! Constructor from number of amplitudes
TLSAmplitudes::TLSAmplitudes(size_t n)
{
  if ( n == 0 )
  {
    throw std::invalid_argument( "n must be a positive integer" );
  }
  // Resize vals array and fill with ones
  vals.reserve(n);
  for (int i = 0; i < n; i++)
  {
    vals.push_back(1.0);
  }
}

//! Constructor from another TLSAmplitudes object
TLSAmplitudes::TLSAmplitudes(const TLSAmplitudes &other)
{
  vals = dblArr1d(other.vals.begin(), other.vals.end());
  round();
}

//! Constructor from array of values
TLSAmplitudes::TLSAmplitudes(const dblArr1d &values)
{
  vals = dblArr1d(values.begin(), values.end());
  round();
}

//! Get the number of decimal places to be used for rounding
int TLSAmplitudes::getPrecision()
{
  return log10(rnd);
}

//! Set the number of decimal places to be used for rounding
void TLSAmplitudes::setPrecision(int decimals)
{
  rnd = pow(10.0, (double)decimals);
}

//! Get default tolerance for determining validity of matrices
double TLSAmplitudes::getTolerance()
{
  return tol;
}

//! Set default tolerance for determining validity of matrices
void TLSAmplitudes::setTolerance(double tolerance)
{
  if (tolerance <= 0.0)
  {
    throw std::invalid_argument( "tolerance must be greater than 0.0" );
  }
  tol = tolerance;
}

//! Indexing operator returns the i'th element of the amplitudes
double TLSAmplitudes::operator[] (const int index)
{
  return vals[index];
}

//! Override + operator for adding two classes together.
TLSAmplitudes& TLSAmplitudes::operator+(const TLSAmplitudes &other) const
{
  TLSAmplitudes& c = *copy();
  c.add(other);
  return c;
}

//! Override * operator for multiplying by a scalar
TLSAmplitudes& TLSAmplitudes::operator*(double scalar) const
{
  TLSAmplitudes& c = *copy();
  c.multiply(scalar);
  return c;
}

//! Addition of another class
void TLSAmplitudes::add(const TLSAmplitudes &other)
{
  if (vals.size()!=other.vals.size())
  {
    throw std::invalid_argument( "TLSAmplitudes must have the same length" );
  }
  vals += other.vals;
}

//! Test if any amplitudes values are nonzero
bool TLSAmplitudes::any(double tolerance)
{
  sanitiseTolerance(&tolerance);
  for (int i = 0; i < vals.size(); i++)
  {
    if (fabs(vals[i]) > tolerance)
    {
      return true;
    }
  }
  return false;
}

//! Create a copy of the class
TLSAmplitudes* TLSAmplitudes::copy() const
{
  return new TLSAmplitudes(*this);
}

//! Description strings of the functions
std::string TLSAmplitudes::getDescription()
{
  return description;
}

//! Get the amplitude values (by value!)
dblArr1d TLSAmplitudes::getValues() const
{
  return vals;
}

//! Get the amplitude values for a selection of indices (by value!)
dblArr1d TLSAmplitudes::getValuesBySelection(const selArr1d &selection)
{
  // Check this is a valid selection
  validateSelection(selection);
  dblArr1d result;
  result.reserve(selection.size());
  for (int i = 0; i < selection.size(); i++)
  {
    size_t ii = selection[i];
    result.push_back(vals[ii]);
  }
  return result;
}

//! Multiplication by a scaler
void TLSAmplitudes::multiply(double scalar)
{
  scale(scalar);
  round();
}

//! Divide the amplitudes by their average, and return multiplier to be applied to the TLSMatrices
double TLSAmplitudes::normalise(double target)
{
  // Target musr be positive
  if ( target <= 0.0 )
  {
    throw std::invalid_argument( "target must be positive" );
  }
  double total = 0.0;
  for (int i = 0; i < vals.size(); i++)
  {
    total += vals[i];
  }
  double mean = total / (double)vals.size();
  double mult = target / mean;
  scale(mult);
  round();
  return 1.0 / mult;
}

//! Count the number of (non-zero) parameters
int TLSAmplitudes::paramCount(bool non_zero)
{
  if (!non_zero)
  {
    return vals.size();
  }
  int result = 0;
  for (int i = 0; i < vals.size(); i++)
  {
    // Perform the comparison exactly, not using tolerance
    if (vals[i]!=0.0) {
      result += 1;
    }
  }
  return result;
}

//! Set all amplitudes to 1.0
void TLSAmplitudes::reset()
{
  for (int i = 0; i < vals.size(); i++)
  {
    vals[i] = 1.0;
  }
}

//! Round all amplitudes to a number of decimal places
void TLSAmplitudes::round()
{
  for (int i = 0; i < vals.size(); i++)
  {
    vals[i] = rint(rnd*vals[i]) / rnd;
  }
}

//! Set matrices vals from an array of the same size as the object
void TLSAmplitudes::setValues(const dblArr1d &values)
{
  // Assert values is same length as internal values
  if (values.size() != vals.size())
  {
    throw std::invalid_argument( "Input array must be the same length as TLSAmplitudes" );
  }
  // Copy across selected values
  for (int i = 0; i < values.size(); i++)
  {
    // Copy across the i-th values to the index indicated by selection
    vals[i] = values[i];
  }
  round();
}

//! Set matrices vals from an array and a selection of indices
void TLSAmplitudes::setValuesBySelection(const dblArr1d &values, const selArr1d &selection)
{
  // Check this is a valid selection
  validateSelection(selection);
  // Assert values is same length as selection
  if (values.size() != selection.size())
  {
    throw std::invalid_argument( "Input values must be the same length as input selection" );
  }
  // Copy across selected values
  for (int i = 0; i < selection.size(); i++)
  {
    // Copy across the i-th values to the index indicated by selection
    size_t ii = selection[i];
    vals[ii] = values[i];
  }
  round();
}

//! Return the number of amplitudes values
int TLSAmplitudes::size() const
{
  return vals.size();
}

//! Get summary string for the amplitudes in the object
std::string TLSAmplitudes::summary()
{
  std::ostringstream buff;
  buff << "> TLS Amplitudes (" << short_description << ")" << std::endl;

  for (int i = 0; i < vals.size(); i++)
  {
    buff << std::endl
         << "    Dataset "
         << std::setw(4)
         << std::noshowpos
         << i+1 << ": "
         << std::setw(10)
         << std::showpos
         << std::setprecision((int)log10(rnd))
         << vals[i];
  }

  return buff.str();
}

void TLSAmplitudes::zeroValues()
{
  dblArr1d zeros(size(), 0.0);
  setValues(zeros);
}

void TLSAmplitudes::zeroNegativeValues()
{
  for (int i = 0; i < vals.size(); i++)
  {
    if (vals[i] < 0.0)
    {
      vals[i] = 0.0;
    }
  }
}

//! Set a tolerance value to the default value if less than zero
void TLSAmplitudes::sanitiseTolerance(double *tolerance)
{
  if (*tolerance < 0.0)
  {
    // Check that tolerance == -1
    if (*tolerance != -1)
    {
      throw std::invalid_argument( "Tolerance provided must either be positive or -1" );
    }
    // Set the pointer value to the internal default value
    *tolerance = tol;
  }
}

void TLSAmplitudes::scale(double multiplier)
{
  vals *= multiplier;
}

void TLSAmplitudes::validateSelection(const selArr1d &selection)
{
  // Throw if no values are passed
  if (selection.size() == 0)
  {
    throw std::invalid_argument( "No indices given for selection" );
  }
  // Assert values is compatible with vals
  if (selection.size() > vals.size())
  {
    throw std::invalid_argument( "Selection indices cannot be longer than TLSAmplitudes" );
  }
  for (int i = 0; i < selection.size(); i++)
  {
    if (selection[i] >= vals.size())
    {
      throw std::invalid_argument( "Selection indices out of range of TLSAmplitudes" );
    }
  }
}

//! Overload that allows left-multiplication of TLSAmplitudes
TLSAmplitudes operator*(const double &v, const TLSAmplitudes &x)
{
  TLSAmplitudes result = x * v;
  return result;
}

//
// TLSMatricesAndAmplitudes
//

//! Constructor from number of amplitudes
TLSMatricesAndAmplitudes::TLSMatricesAndAmplitudes(size_t n)
  : lbl(-1)
{
  mats = new TLSMatrices();
  amps = new TLSAmplitudes(n);
}

//! Constructor from other class
TLSMatricesAndAmplitudes::TLSMatricesAndAmplitudes(const TLSMatricesAndAmplitudes& other)
{
  mats = other.getMatricesConst()->copy();
  amps = other.getAmplitudesConst()->copy();
  lbl = other.lbl;
}

//! Constructor from matrix-amplitude pair
TLSMatricesAndAmplitudes::TLSMatricesAndAmplitudes(TLSMatrices& matrices, TLSAmplitudes& amplitudes)
  : lbl(-1)
{
  mats = &matrices;
  amps = &amplitudes;
}

//! Constructor from matrix and amplitude arrays
TLSMatricesAndAmplitudes::TLSMatricesAndAmplitudes(const dblArr1d& matrix_values, const dblArr1d& amplitude_values)
  : lbl(-1)
{
  if ( matrix_values.size() != 21 )
  {
    throw std::invalid_argument( "Matrix values must have length 21" );
  }
  if ( amplitude_values.size() == 0 )
  {
    throw std::invalid_argument( "Amplitude values must have length greater than 0" );
  }
  mats = new TLSMatrices(matrix_values);
  amps = new TLSAmplitudes(amplitude_values);
}

//! Create a copy of the class
TLSMatricesAndAmplitudes* TLSMatricesAndAmplitudes::copy() const
{
  return new TLSMatricesAndAmplitudes(*this);
}

//! Return a reference to the owned matrices class
TLSMatrices* TLSMatricesAndAmplitudes::getMatrices()
{
  return mats;
}

const TLSMatrices* TLSMatricesAndAmplitudes::getMatricesConst() const
{
  return mats;
}

//! Return a reference to the owned amplitudes class
TLSAmplitudes* TLSMatricesAndAmplitudes::getAmplitudes()
{
  return amps;
}

const TLSAmplitudes* TLSMatricesAndAmplitudes::getAmplitudesConst() const
{
  return amps;
}

//! Return a list of amplitude-multiplied TLSMatrices
af::shared<TLSMatrices> TLSMatricesAndAmplitudes::expand()
{
  // Dereference internal pointers
  TLSMatrices& m = *mats;
  TLSAmplitudes& a = *amps;
  // Output array
  af::shared<TLSMatrices> results;
  results.reserve(a.size());
  for (int i = 0; i < a.size(); i++)
  {
    TLSMatrices mult_mats = m * a[i];
    results.push_back(mult_mats);
  }
  return results;
}

//! Return a list of amplitude-multiplied TLSMatrices by selection
af::shared<TLSMatrices> TLSMatricesAndAmplitudes::expand(const selArr1d &selection)
{
  // Dereference internal pointers
  TLSMatrices& m = *mats;
  TLSAmplitudes& a = *amps;
  // Get list of amplitudes
  dblArr1d sel_a = a.getValuesBySelection(selection);
  // Output array
  af::shared<TLSMatrices> results;
  results.reserve(sel_a.size());
  for (int i = 0; i < sel_a.size(); i++)
  {
    TLSMatrices mult_mats = m * sel_a[i];
    results.push_back(mult_mats);
  }
  return results;
}

bool TLSMatricesAndAmplitudes::isValid(double tolerance)
{
  const af::shared<TLSMatrices> expanded = expand();
  for (int i = 0; i < expanded.size(); i++)
  {
    const TLSMatrices& m = expanded[i];
    bool valid = m.isValid(tolerance);
    if (!valid) { return false; }
  }
  return true;
}

bool TLSMatricesAndAmplitudes::isValid(const selArr1d &selection, double tolerance)
{
  const af::shared<TLSMatrices> expanded = expand(selection);
  for (int i = 0; i < expanded.size(); i++)
  {
    const TLSMatrices& m = expanded[i];
    bool valid = m.isValid(tolerance);
    if (!valid) { return false; }
  }
  return true;
}

bool TLSMatricesAndAmplitudes::isNull(double matricesTolerance, double amplitudesTolerance)
{
  bool m_any = mats->any("TLS", matricesTolerance);
  bool a_any = amps->any(amplitudesTolerance);
  return !(m_any && a_any);
}

int TLSMatricesAndAmplitudes::paramCount(bool free, bool non_zero)
{
  return mats->paramCount(free, non_zero) +
         amps->paramCount(non_zero);
}

double TLSMatricesAndAmplitudes::normaliseByAmplitudes(double target)
{
  double mult = amps->normalise(target);
  if (mult > 0.0)
  {
    mats->multiply(mult);
  }
  return mult;
}

double TLSMatricesAndAmplitudes::normaliseByMatrices(const vecArrNd &sites_carts, const vecArr1d &origins, double target)
{
  // Validate input
  validateSitesAndOrigins(sites_carts, origins);
  // Extract the number of atoms/datasets from size of input array
  size_t n_dst = sites_carts.accessor().all()[0];
  size_t n_atm = sites_carts.accessor().all()[1];
  // Create a 1-d list of the coordinates (with origins subtracted)
  vecArr1d flattened;
  for (int i = 0; i < n_dst; i++)
  {
    // Get the origin for the "current" dataset
    vec current_origin = origins[i];
    // Iterate through each atom and subtract origin
    for (int j = 0; j < n_atm; j++)
    {
      vec shifted = sites_carts(i,j) - current_origin;
      flattened.push_back(shifted);
    }
  }
  // Normalise matrices and extract multiplier
  vec null_origin(0.0, 0.0, 0.0);
  double mult = mats->normalise(flattened, null_origin, target);
  // Apply multiplier to amplitudes
  if (mult > 0.0)
  {
    amps->multiply(mult);
  }
  return mult;
}

void TLSMatricesAndAmplitudes::reset()
{
  mats->reset();
  amps->reset();
}

void TLSMatricesAndAmplitudes::setLabel(int label)
{
  lbl = label;
}

std::string TLSMatricesAndAmplitudes::summary()
{
  std::ostringstream buff;
  if (lbl > 0)
  {
    buff << "> TLS Mode " << lbl << std::endl;
  } else {
    buff << "> TLS Mode Summary" << std::endl;
  }
  buff << std::endl << "  " << find_and_replace(mats->summary(), "\n", "\n  ");
  buff << std::endl << "  " << find_and_replace(amps->summary(), "\n", "\n  ");
  return buff.str();
}

symArrNd TLSMatricesAndAmplitudes::uijs(const vecArrNd &sites_carts, const vecArr1d &origins)
{
  // Check compatible with the amplitudes of this object
  if (origins.size() != amps->size())
  {
    throw std::invalid_argument( "Mismatch between the size of TLSAmplitudes and the input arrays" );
  }
  // Extract list of TLSMatrices
  af::shared<TLSMatrices> mat_list = expand();
  return uijs(sites_carts, origins, mat_list);
}

symArrNd TLSMatricesAndAmplitudes::uijs(const vecArrNd &sites_carts, const vecArr1d &origins, const selArr1d &selection)
{
  // Check selection is the correct length
  if (origins.size()!=selection.size())
  {
    throw std::invalid_argument( "Mismatch between the size of selection and the input arrays" );
  }
  // Extract list of TLSMatrices
  af::shared<TLSMatrices> mat_list = expand(selection);
  return uijs(sites_carts, origins, mat_list);
}

void TLSMatricesAndAmplitudes::resetIfNull(double matricesTolerance, double amplitudesTolerance)
{
  if ( isNull(matricesTolerance, amplitudesTolerance) )
  {
    reset();
  }
}

symArrNd TLSMatricesAndAmplitudes::uijs(const vecArrNd &sites_carts, const vecArr1d &origins, const af::shared<TLSMatrices> &tls_matrices)
{
  // Validate input
  validateSitesAndOrigins(sites_carts, origins);
  // Validate length of input list
  if (origins.size() != tls_matrices.size())
  {
    throw std::invalid_argument( "Mismatch between the size of tls_matrices and size of sites_carts/origins" );
  }
  // Check everything is compatible
  size_t n_dst = sites_carts.accessor().all()[0];
  size_t n_atm = sites_carts.accessor().all()[1];
  symArrNd result_uijs(af::flex_grid<>(n_dst, n_atm)); // do not need to initialise to zeros as all elements filled
  for (int i = 0; i < n_dst; i++)
  {
    // Extract matrices from list
    const TLSMatrices& curr_mat = tls_matrices[i];
    // Copy sites for this dataset
    vecArr1d curr_sites = vecArr1d(&sites_carts(i,0), &sites_carts(i, n_atm));
    // Calculate uijs for this dataset
    symArr1d curr_uijs = curr_mat.uijs(curr_sites, origins[i]);
    // Copy output uij to output array
    memcpy(&result_uijs(i,0), &curr_uijs[0], sizeof(sym) * n_atm);
  }
  return result_uijs;
}

//
// TLSMatricesAndAmplitudesList
//

//! Constructor from length and number of amplitudes
TLSMatricesAndAmplitudesList::TLSMatricesAndAmplitudesList(size_t length, size_t n_amplitudes)
{
  initialiseList(length, n_amplitudes);
}

//! Constructor for existing TLSMatricesAndAmplitudesList object
TLSMatricesAndAmplitudesList::TLSMatricesAndAmplitudesList(const TLSMatricesAndAmplitudesList &other)
{
  list.reserve(other.size());
  for (int i = 0; i < other.size(); i++)
  {
    // copy the object
    TLSMatricesAndAmplitudes* ma = other.list[i]->copy();
    ma->setLabel(i+1);
    list.push_back(ma);
  }
}

//! Constructor from two arrays of matrices/amplitudes
TLSMatricesAndAmplitudesList::TLSMatricesAndAmplitudesList(const dblArrNd& matrix_values, const dblArrNd& amplitude_values)
{
  // Check validity of input arrays
  if (matrix_values.accessor().nd() != 2)      { throw std::invalid_argument( "matrix_values must be 2-dimensional array of size (n_sets, 21)" ); }
  if (amplitude_values.accessor().nd() != 2)   { throw std::invalid_argument( "amplitude_values must be 2-dimensional array of size (n_sets, n_amplitudes)" ); }
  if (matrix_values.accessor().all()[1] != 21) { throw std::invalid_argument( "The length of the second axis of matrix_values must be 21" ); }

  // Get size of arrays (will set size of list, etc)
  size_t n_sets = matrix_values.accessor().all()[0];
  size_t n_amps = amplitude_values.accessor().all()[1];

  // Validate against other dimensions that should be the same length
  if (amplitude_values.accessor().all()[0] != n_sets)
  {
    throw std::invalid_argument( "The length of the first axis of matrix_values and amplitude_values must match (number of matrix-amplitude pairs)" );
  }

  // Initialise the holder list to the right length
  initialiseList(n_sets, n_amps);
  // Set the values manually
  for (int i = 0; i < list.size(); i++)
  {
    // Extract the values for this mode
    dblArr1d mat_vals(&matrix_values(i,0), &matrix_values(i,21));
    dblArr1d amp_vals(&amplitude_values(i,0), &amplitude_values(i,n_amps));
    // Set the values for this mode
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->getMatrices()->setValuesByString(mat_vals, "TLS", true);
    ma->getAmplitudes()->setValues(amp_vals);
  }
}

TLSMatricesAndAmplitudes* TLSMatricesAndAmplitudesList::operator[] (const int index)
{
  return get(index);
}

TLSMatricesAndAmplitudesList* TLSMatricesAndAmplitudesList::copy() const
{
  return new TLSMatricesAndAmplitudesList(*this);
}

TLSMatricesAndAmplitudes* TLSMatricesAndAmplitudesList::get(const int index)
{
  validateIndex(index);
  return list[index];
}

TLSMatricesAndAmplitudes* TLSMatricesAndAmplitudesList::getConst(const int index) const
{
  validateIndex(index);
  return list[index];
}

bool TLSMatricesAndAmplitudesList::isNull(double matricesTolerance, double amplitudesTolerance)
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    bool mode_is_null = ma->isNull(matricesTolerance, amplitudesTolerance);
    if (!mode_is_null)
    {
      return false;
    }
  }
  return true;
}

int TLSMatricesAndAmplitudesList::paramCount(bool free, bool non_zero)
{
  int result = 0;
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    result += ma->paramCount(free, non_zero);
  }
  return result;
}

void TLSMatricesAndAmplitudesList::normaliseByAmplitudes(double target)
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->normaliseByAmplitudes(target);
  }
}

void TLSMatricesAndAmplitudesList::normaliseByMatrices(const vecArrNd &sites_carts, const vecArr1d &origins, double target)
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->normaliseByMatrices(sites_carts, origins, target);
  }
}

void TLSMatricesAndAmplitudesList::reset()
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->getAmplitudes()->reset();
    ma->getMatrices()->reset();
  }
}

void TLSMatricesAndAmplitudesList::resetMatrices()
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->getMatrices()->reset();
  }
}

void TLSMatricesAndAmplitudesList::resetNullModes(double matricesTolerance, double amplitudesTolerance)
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->resetIfNull(matricesTolerance, amplitudesTolerance);
  }
}

size_t TLSMatricesAndAmplitudesList::size() const
{
  return list.size();
}

std::string TLSMatricesAndAmplitudesList::summary()
{
  std::ostringstream buff;
  for (int i = 0; i < size(); i++)
  {
    buff << list[i]->summary();
    if (i>0)
    {
      buff << "\n";
    }
  }
  return buff.str();
}

symArrNd TLSMatricesAndAmplitudesList::uijs(const vecArrNd &sites_carts, const vecArr1d &origins)
{
  // Validate input
  validateSitesAndOrigins(sites_carts, origins);
  // Extract array sizes
  size_t n_dst = sites_carts.accessor().all()[0];
  size_t n_atm = sites_carts.accessor().all()[1];
  // Prepare output array
  symArrNd result_uijs(af::flex_grid<>(n_dst, n_atm), sym(0.0,0.0,0.0,0.0,0.0,0.0)); // need to initialise to zeros
  // Add contributions from each mode
  for (int i = 0; i < size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    // Save time by skipping null modes
    if ( ma->isNull() )
    {
      continue;
    }
    // Calculate Uijs for this mode
    symArrNd this_uijs = ma->uijs(sites_carts, origins);
    // Add to the output results array
    for (int j = 0; j < this_uijs.size(); j++)
    {
      result_uijs[j] += this_uijs[j];
    }
  }
  return result_uijs;
}

symArrNd TLSMatricesAndAmplitudesList::uijs(const vecArrNd &sites_carts, const vecArr1d &origins, const selArr1d &selection)
{
  // Validate input
  validateSitesAndOrigins(sites_carts, origins);
  // Extract array sizes
  size_t n_dst = sites_carts.accessor().all()[0];
  size_t n_atm = sites_carts.accessor().all()[1];
  // Prepare output array
  symArrNd result_uijs(af::flex_grid<>(n_dst, n_atm), sym(0.0,0.0,0.0,0.0,0.0,0.0)); // need to initialise to zeros
  // Add contributions from each mode
  for (int i = 0; i < size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    // Save time by skipping null modes
    if ( ma->isNull() )
    {
      continue;
    }
    // Calculate Uijs for this mode
    symArrNd this_uijs = ma->uijs(sites_carts, origins, selection);
    // Add to the output results array
    for (int j = 0; j < this_uijs.size(); j++)
    {
      result_uijs[j] += this_uijs[j];
    }
  }
  return result_uijs;
}

void TLSMatricesAndAmplitudesList::zeroAmplitudes(const selArr1d &selection)
{
  // Assert values is compatible with vals
  validateSelection(selection);
  // Iterate through the selected modes and set amplitudes to zero
  for (int i = 0; i < selection.size(); i++)
  {
    size_t curr = selection[i];
    TLSMatricesAndAmplitudes* ma = list[curr];
    ma->getAmplitudes()->zeroValues();
  }
}

void TLSMatricesAndAmplitudesList::zeroNegativeAmplitudes()
{
  for (int i = 0; i < list.size(); i++)
  {
    TLSMatricesAndAmplitudes* ma = list[i];
    ma->getAmplitudes()->zeroNegativeValues();
  }
}

void TLSMatricesAndAmplitudesList::initialiseList(size_t length, size_t n_amplitudes)
{
  if ( list.size() > 0 )
  {
    throw std::invalid_argument( "List is already initialised!" );
  }
  if ( length == 0 ) // size_t, so only need to check for 0-length
  {
    throw std::invalid_argument( "Length of list <length> must be one or greater" );
  }
  if ( n_amplitudes == 0 ) // size_t, so only need to check for 0-length
  {
    throw std::invalid_argument( "Number of amplitudes <n_amplitudes> must be one or greater" );
  }
  list.reserve(length);
  for (int i = 0; i < length; i++)
  {
    TLSMatricesAndAmplitudes* ma = new TLSMatricesAndAmplitudes(n_amplitudes);
    ma->setLabel(i+1);
    list.push_back(ma);
  }
}

void TLSMatricesAndAmplitudesList::validateIndex(size_t index) const
{
  if ( index >= list.size() )
  {
    throw std::invalid_argument( "index out of range of TLSMatricesAndAmplitudesList" );
  }
}

void TLSMatricesAndAmplitudesList::validateSelection(const selArr1d &selection)
{
  if (selection.size() > size())
  {
    throw std::invalid_argument( "Selection indices cannot be longer than TLSMatricesAndAmplitudesList" );
  }
  for (int i = 0; i < selection.size(); i++)
  {
    if (selection[i] >= size())
    {
      throw std::invalid_argument( "Selection indices out of range of TLSMatricesAndAmplitudesList" );
    }
  }
}

} } } // close namepsace mmtbx/tls/utils
