# Documentation for Split Spots Program

This program runs live when user executes dials.image_viewer.

There is a checkbox for showing close spots and when it is checked, the main 
file is executed and the results are displayed on the diffraction image.


## Main File: close_spots.py

There are a total of 13 functions in this file.

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
        by using eiger2cbf and extracts data from the cbf

        h5_path: str
                full path to master h5 file related to experiment in current context

        returns wavelength and sample-detector distance
```

```python
def find_distance(p, q)

        this function finds the distance between two points using the euclidean distance
        formula

        p: list
                two elements x1, y1

        q: list
                two elements x2, y2

        returns the calculated distance between points p and q
```

```python
def units_distance(p, q, distance)

        checks to see if points p and q are within a specific radius

        Note: this function is here for future purposes. The reflections are further apart
        as one walks outward

        p: list
                two elements x1, y1

        q: list
                two elements x2, y2

        distance: float
                the upper threshold to calculating distance between p and q
```

### spotfinder_frame.py was also modified
##### the following shows where I inserted lines of code and in what functions

```python
class SpotFrame(XrayFrame):
        def __init__(self, *args, **kwds):
                ...
                self.shoebox_layer = None
                self.close_spots_layer = None # <-- THIS I ADDED - JAD
                self.ctr_mass_layer = None
                ...
                self.show_shoebox_timer = time_log("show_shoebox")
                self.show_close_spots_timer = time_log("show_close_spots") # <-- THIS I ADDED - JAD
                self.show_max_pix_timer = time_log("show_max_pix")
                ...
                self.draw_shoebox_timer = time_log("draw_shoebox")
                self.draw_close_spots_timer = time_log("draw_close_spots") # <-- THIS I ADDED - JAD
                self.draw_max_pix_timer = time_log("draw_max_pix")

        def update_settings(self, layout=True):
                ...
                if self.close_spots_layer is not None:                         # <-- THIS I ADDED - JAD
                self.pyslip.DeleteLayer(self.close_spots_layer, update=False)  # <-- THIS I ADDED - JAD
                self.close_spots_layer = None                                  # <-- THIS I ADDED - JAD
                ...
                # I ADDED THIS BELOW - JAD
                if self.settings.show_close_spots and len(close_spot_data):
                        self.draw_close_spots_timer.start()
                        self.close_spots_layer = self.pyslip.AddPointLayer(
                                close_spot_data,
                                color="orange",
                                radius=2,
                                renderer=self.pyslip.LightweightDrawPointLayer,
                                show_levels=[-3, -2, -1, 0, 1, 2, 3, 4, 5],
                                name="<close_spot_layer>",
                                update=False,
                        )
                        self.draw_close_spots_timer.stop()
                # I ENDED THIS ABOVE - JAD

        def get_spotfinder_data(self):
                ...
                close_spot_dict = {"width": 2, "color": "#FF4C00", "closed": False} # <-- THIS I ADDED - JAD
                shoebox_dict = {"width": 2, "color": "#0000FFA0", "closed": False}
                ...
                vector_text_data = []
                close_spot_data = [] # <-- THIS I ADDED - JAD
                reflections_data = {'xyzobs.px.value': []} # <-- THIS I ADDED - JAD
                detector = self.pyslip.tiles.raw_image.get_detector()
                ...
                # I ADDED THIS BELOW - JAD
                if self.settings.show_close_spots:
                        self.show_close_spots_timer.start()
                        print("JAD: self.experiments = {}".format(self.experiments))

                        import time
                        start_time = time.time()
                        for reflection in selected.rows():
                                reflections_data['xyzobs.px.value'].append(reflection['xyzobs.px.value'])

                        from dials.util import close_spots
                        closest_points = close_spots.main(reflections_data, dist=10)

                        #reflections = flex.reflection_table.empty_standard(len(closest_points))
                        reflections = self.reflections[0]
                        print("reflection object was made = {}".format(reflections))

                        count = 0
                        for i, reflection in enumerate(reflections['xyzobs.px.value']):
                                if reflection in closest_points:
                                        print("reflection in closest points !!!!!!!")
                                        print("reflection {} => {}".format((i+1), reflection))
                                        count += 1
                                else:
                                        reflections['xyzobs.px.value'][i] = (0,0,0)
                        print("count is {}".format(count))

                        for refl in reflections['xyzobs.px.value']:
                                print("refl = {}".format(refl))
                
                        print("self.experiments = {}".format(self.experiments[0]))
                        reflections.centroid_px_to_mm(self.experiments[0])
                        reflections.map_centroids_to_reciprocal_space(self.experiments[0])

                        for key in dict(reflections):
                                print("key = {}".format(key))

                        for val in reflections['rlp']:
                                print('value of rlp = {}'.format(val))

                        for centroid in closest_points:
                                x, y = map_coords(
                                        centroid[0], centroid[1], 0
                                )
                                close_spot_data.append((x,y))
                        print("time taken for close spots: {:.2f}ms\n\n".format((time.time() - start_time) * 1000))

                        # self.reflections.map_centroids_to_reciprocal_space(self.experiments)
                        self.show_close_spots_timer.stop()
                # I ENDED THIS ABOVE - JAD
                ...
                return group_args(
                        ...
                        vector_text_data=vector_text_data,
                        close_spot_data=close_spot_data # <-- THIS I ADDED - JAD
                )

class SpotSettingsPanel(wx.Panel):
        def __init__(self, *args, **kwargs):
                ...
                self.settings.show_spotfinder_spots = False
                self.settings.show_close_spots = self.params.show_close_spots # <-- THIS I ADDED - JAD
                self.settings.show_dials_spotfinder_spots = True
                ...
                # JAD Close spots points
                self.close_spots = wx.CheckBox(self, -1, "Show close spots")        # <-- THIS I ADDED - JAD
                self.close_spots.SetValue(self.settings.show_close_spots)           # <-- THIS I ADDED - JAD
                grid.Add(self.close_spots, 0, wx.ALL | wx.ALIGN_CENTER_VERTICAL, 5) # <-- THIS I ADDED - JAD
                ...
                self.Bind(wx.EVT_CHECKBOX, self.OnUpdate, self.show_basis_vectors)
                self.Bind(wx.EVT_CHECKBOX, self.OnUpdate, self.close_spots) # <-- THIS I ADDED - JAD
                self.Bind(wx.EVT_CHECKBOX, self.OnUpdateShowMask, self.show_mask)

        def collect_values(self):
                ...
                self.settings.show_shoebox = self.shoebox.GetValue()
                self.settings.show_close_spots = self.close_spots.GetValue() # <-- THIS I ADDED - JAD
                self.settings.show_indexed = self.indexed.GetValue()
        
        def OnClearAll(self, event):
                ...
                for btn in (
                        ...
                        self.shoebox,
                        self.close_spots, # <-- THIS I ADDED - JAD
                        self.predictions,
                        ...
                ):
                
```