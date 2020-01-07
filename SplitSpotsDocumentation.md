# Documentation for Split Spots Program

## Main File: close_spots.py

There are a total of 15 functions in this file.

All functions are not necessary to be used but some functions can be used for
further analysis and processing of data.

### Main functions that are used:
  
```python
def main(reflections, dist=None)  

        this is where all the functions are called in order to find
        the spots that are close to each other

        reflections: dict
                the key in this dictionary is xyzobs.px.value and the
                value is a list of all reflections in tuple format

        dist: integer or float, optional
                the desired distance user wants program to use

        returns flex.vec2_double() where each element in this object
                is a tuple of size 2 => (x_val, y_val)
```

```python
def get_xyz_coords(reflections)

        this function extracts the xyz values from the reflections object
        and saves it in a dictionary
        
        reflections: dict
                the key in this dictionary is xyzobs.px.value and the
                value is a list of all reflections in tuple format

        returns a dictionary where the key is the X-VALUE and the
                value are the y and z coordinates
```

```python
def order_dictionary(dictionary)

        this function orders the points by x-value

        dictionary: dict
                dictionary that was the output from function 2 above

        returns an ordered dictionary where the ordering based on x-values
```

```python
def add_index_to_pairs(ordered_points)

        
        this function adds an integer index to each reflection for
        the euclidean_distance() function 

        ordered_points: dict
                this object was the output of function 3 above

        returns a dictionary where the key is an integer for indexing,
                the inner key is the x-value and the value is the y, z coordinates
```

```python
def euclidean_distance(ordered_points, distance=7)


        this is the function where the algorithm for seeing if two spots are close
        is used. The algorithm is simple and checks to see if two points are within
        the user-specified or default distance using the euclidean distance formula

        ordered_points: dict
                this object is the output from function 4 above
        
        distance: default 7, optional
                if user does not specify the distance, it will use 7 as a distance
```

```python
def save_spots_in_vec3(close_points)

        this function saves the list of points as a flex.vec3_double() object

        close_points: list
                each element is a list with 3 coordinates [x, y, z]

        returns vec3 object with all the points saved
```

### Other functions used in program:

```python
def get_detector_distance_n_wavelength(filename=None)

        this function works to extract the sample-detector distance and wavelength of experiment
        and prints out the results

        filename: optional, str
                the filename should be the one outputted from dials.import
                        import.expt or imported_experiments.json
```

```python
def get_file(filename=None)


        this function finds the correct file outputted from dials.import if filename is equal to None

        filename: optional
                this could be imported.expt or imported_experiments.json

        returns the correct filename to use for extraction of data
```

```python
def get_base_path(filename)

        this function gets the directory to where the master h5 or cbf files are to be found

        returns a string
``` 

```python
def get_distance_n_wavelength_cbf_version(cbf_path)

        this function searches for the cbf file that contains info on the experiment
        and finds the wavelength and sample-detector distance

        cbf_path: str
                full path to cbf file related to experiment in current context

        returns wavelength and sample-detector distance
```

```python
def get_distance_n_wavelength_h5_version(h5_path)

        this function searches for the master h5 file and converts it into a cbf
        user eiger2cbf and extracts data from the cbf

        h5_path: str
                full path to master h5 file related to experiment in current context

        returns wavelength and sample-detector distance
```