# Documentation for Split Spots Program

## Main File: close_spots.py

There are a total of 15 functions in this file.

All functions are not necessary to be used but some functions can be used for
further analysis and processing of data.

Main functions that are used:
1. main(reflections, dist=None)

        reflections: dict
                the key in this dictionary is xyzobs.px.value and the
                value is a list of all reflections in tuple format

        dist: integer or float, optional
                the desired distance user wants program to use

        returns flex.vec2_double() where each element in this object
                is a tuple of size 2 => (x_val, y_val)

2. get_xyz_coords(reflections)
        
        reflections: dict
                the key in this dictionary is xyzobs.px.value and the
                value is a list of all reflections in tuple format

        returns a dictionary where the key is the X-VALUE and the
                value are the y and z coordinates

3. order_dictionary(dictionary)

        dictionary: dict
                dictionary that was received from function 2 above

        returns an ordered dictionary
