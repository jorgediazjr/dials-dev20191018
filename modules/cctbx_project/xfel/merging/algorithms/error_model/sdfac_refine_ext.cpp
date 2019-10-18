#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <dials/array_family/reflection_table.h>
#include <cctbx/miller.h>

namespace xfel {
namespace merging {
namespace error_model {
namespace sdfac_refine {

typedef
 scitbx::af::versa<cctbx::miller::index<>, scitbx::af::flex_grid<> > shared_miller;

using namespace dials::af;

static scitbx::af::shared<double>
compute_normalized_deviations(reflection_table ISIGI, shared_miller hkl_list) {
  /*
   * This formulation of the normalized deviations of a set of intensities and sigmas is similar to that
   * described in Evans 2011, but includes the nn term as currently implmented by aimless
   *
   */
  SCITBX_ASSERT(ISIGI.contains("scaled_intensity"));
  SCITBX_ASSERT(ISIGI.contains("meanprime_scaled_intensity"));
  SCITBX_ASSERT(ISIGI.contains("isigi"));
  SCITBX_ASSERT(ISIGI.contains("nn"));

  scitbx::af::shared<double>         result(ISIGI.size(), 0);
  scitbx::af::const_ref<double>      scaled_intensity = ISIGI["scaled_intensity"];
  scitbx::af::const_ref<double>      meanprime_scaled_intensity = ISIGI["meanprime_scaled_intensity"];
  scitbx::af::const_ref<double>      isigi = ISIGI["isigi"];
  scitbx::af::const_ref<double>      nn = ISIGI["nn"];

  double sigma;
  #pragma omp parallel for private(sigma)
  for (int i = 0; i < ISIGI.size(); i++) {
    if (isigi[i] == 0 || scaled_intensity[i] == 0) continue;
    sigma = scaled_intensity[i] / isigi[i];
    result[i] = std::sqrt(nn[i]) * (scaled_intensity[i] - meanprime_scaled_intensity[i]) / sigma;
  }
  return result;
}

void
apply_sd_error_params(reflection_table ISIGI, const double sdfac, const double sdb, const double sdadd, const bool squared_params) {
  /*
   * Apply a set of sd params (sdfac, sdb and sdd) to an ISIGI reflection table

   Squared not only uses the squared formulation of sigma', but also fixes 2 bugs:
   1) Use meanI not meanIprime
   2) When returning isigi, don't multiply by slope

   If using squared, it is assumed that sdfac, sdb and sdadd have already been squared
   */
  SCITBX_ASSERT(ISIGI.contains("scaled_intensity"));
  SCITBX_ASSERT(ISIGI.contains("mean_scaled_intensity"));
  SCITBX_ASSERT(ISIGI.contains("meanprime_scaled_intensity"));
  SCITBX_ASSERT(ISIGI.contains("isigi"));
  SCITBX_ASSERT(ISIGI.contains("slope"));
  SCITBX_ASSERT(ISIGI.contains("miller_id"));

  scitbx::af::const_ref<double>      scaled_intensity = ISIGI["scaled_intensity"];
  scitbx::af::const_ref<double>      mean_scaled_intensity = ISIGI["mean_scaled_intensity"];
  scitbx::af::const_ref<double>      meanprime_scaled_intensity = ISIGI["meanprime_scaled_intensity"];
  scitbx::af::ref<double>            isigi = ISIGI["isigi"];
  scitbx::af::const_ref<double>      slope = ISIGI["slope"];
  scitbx::af::shared<double>         sigmas(ISIGI.size(), 0);
  scitbx::af::const_ref<std::size_t> miller_id = ISIGI["miller_id"];

  std::size_t max_miller_id = scitbx::af::max(miller_id);
  scitbx::af::shared<std::size_t>    n_refl(max_miller_id+1, 0);
  scitbx::af::shared<double>         isum(max_miller_id+1, 0);

  double tmp = 0;
  double sigma_corrected = 0;
  double meanI, meanIprime, minimum;
  #pragma omp parallel for private(meanI, meanIprime, minimum)
  for (int i = 0; i < ISIGI.size(); i++) {
    // scaled intensity (iobs/slope)
    // corrected sigma (original sigma/slope)
    sigmas[i] = scaled_intensity[i] / isigi[i];

    // apply correction parameters
    if (squared_params) {
      // use meanI, which is the mean of all observations of this hkl
      meanI = mean_scaled_intensity[i];
      tmp = std::pow(sigmas[i],2) + sdb * meanI + sdadd * std::pow(meanI,2);
    }
    else {
      // use meanIprime, which for each observation, is the mean of all other observations of this hkl
      meanIprime = meanprime_scaled_intensity[i];
      tmp = std::pow(sigmas[i],2) + sdb * meanIprime + std::pow(sdadd*meanIprime,2);
    }

    // avoid rare negatives
    minimum = 0.1 * std::pow(sigmas[i],2);
    if (tmp < minimum)
      tmp = minimum;

    if (squared_params) {
      sigma_corrected = std::sqrt(sdfac * tmp);
      SCITBX_ASSERT(sigma_corrected != 0.0);
      isigi[i] = scaled_intensity[i] / sigma_corrected;
    }
    else {
      sigma_corrected = sdfac * std::sqrt(tmp);
      SCITBX_ASSERT(sigma_corrected != 0.0);
      isigi[i] = scaled_intensity[i] * slope[i]/ sigma_corrected;
    }
  }
}

scitbx::af::shared<double> df_dpsq(scitbx::af::shared<double>all_sigmas_normalized,
                                   scitbx::af::shared<double>sigma_prime,
                                   scitbx::af::shared<double>dsigmasq_dpsq,
                                   reflection_table ISIGI,
                                   scitbx::af::shared<int>bin_indices,
                                   size_t n_bins) {
  scitbx::af::shared<double> g(n_bins, 0);
  scitbx::af::shared<double> dsigmanormsq_dpsq(ISIGI.size());

  scitbx::af::const_ref<double> scaled_intensity = ISIGI["scaled_intensity"];
  scitbx::af::const_ref<double> meanprime_scaled_intensity = ISIGI["meanprime_scaled_intensity"];
  scitbx::af::const_ref<double> nn = ISIGI["nn"];

  scitbx::af::shared<int> counts(n_bins);
  scitbx::af::shared<double> bnssq(n_bins);
  scitbx::af::shared<double> t3(n_bins);

  double tmp, c; int b;
  // OpenMP doesn't work here. Need a reduction. Easy in OpenMP 4.5 which isn't available yet for my gcc
  for (int i = 0; i < ISIGI.size(); i++) {
    tmp = scaled_intensity[i]-meanprime_scaled_intensity[i];
    c = nn[i] * tmp * tmp;
    dsigmanormsq_dpsq[i] = -c / std::pow(sigma_prime[i], 4) * dsigmasq_dpsq[i];

    b = bin_indices[i];
    counts[b]++;
    bnssq[b] += all_sigmas_normalized[i] * all_sigmas_normalized[i];
    t3[b] += dsigmanormsq_dpsq[i];
  }

  int n; double t1, t2;
  #pragma omp parallel for private(n, t1, t2)
  for (int b = 0; b < n_bins; b++) {
    n = counts[b];

    if (!(n == 0 || bnssq[b] == 0.)) {
      bnssq[b] /= n;
      t1 = 2 * (1 - std::sqrt(bnssq[b]));
      t2 = -0.5 / std::sqrt(bnssq[b]);
      t3[b] /= n;
      g[b] = t1 * t2 * t3[b];
    }
  }
  return g;
}

/* Jiffy function to compute statistics needed downstream
  For every observation, computes:
  mean_scaled_intensity: mean of all observations of this miller index
  meanprime_scaled_intensity: mean of all observations of this miller index except this observation
  n_refl: count of observed reflections for this miller index
  nn: n_refl-1/n_refl
*/
void setup_isigi_stats(reflection_table ISIGI, scitbx::af::const_ref<cctbx::miller::index<> > indices) {
  scitbx::af::shared<double> sumI                = scitbx::af::shared<double>(indices.size(), 0);
  scitbx::af::shared<double> n_refl              = scitbx::af::shared<double>(indices.size(), 0);
  scitbx::af::const_ref<size_t> miller_id        = ISIGI["miller_id"];
  scitbx::af::const_ref<double> scaled_intensity = ISIGI["scaled_intensity"];

  for (size_t i = 0; i < ISIGI.size(); i++) {
    size_t hkl_id = miller_id[i];
    sumI[hkl_id] += scaled_intensity[i];
    n_refl[hkl_id] += 1;
  }

  scitbx::af::shared<double> all_meanI      = scitbx::af::shared<double>(ISIGI.size(), 0);
  scitbx::af::shared<double> all_n_refl     = scitbx::af::shared<double>(ISIGI.size(), 0);
  scitbx::af::shared<double> nn             = scitbx::af::shared<double>(ISIGI.size(), 0);
  scitbx::af::shared<double> all_imeanprime = scitbx::af::shared<double>(ISIGI.size(), 0);
  for (size_t i = 0; i < ISIGI.size(); i++) {
    size_t hkl_id = miller_id[i];
    all_meanI[i] = sumI[hkl_id]/n_refl[hkl_id];
    all_n_refl[i] = n_refl[hkl_id];
    nn[i] = (n_refl[hkl_id]-1)/n_refl[hkl_id];
    SCITBX_ASSERT(n_refl[hkl_id] > 0);
    if (n_refl[hkl_id] > 1)
      all_imeanprime[i] = (sumI[hkl_id]-scaled_intensity[i])/(n_refl[hkl_id]-1);
  }
  ISIGI["mean_scaled_intensity"] = all_meanI;
  ISIGI["n_refl"] = all_n_refl;
  ISIGI["nn"] = nn;
  ISIGI["meanprime_scaled_intensity"] = all_imeanprime ;
}

namespace boost_python { namespace {
  void
  init_module() {
    using namespace boost::python;
    def("compute_normalized_deviations", &compute_normalized_deviations);
    def("apply_sd_error_params", &apply_sd_error_params);
    def("df_dpsq", &df_dpsq);
    def("setup_isigi_stats", &setup_isigi_stats);
}
}}
}}}} // namespace

BOOST_PYTHON_MODULE(xfel_sdfac_refine_ext)
{
  xfel::merging::error_model::sdfac_refine::boost_python::init_module();

}
