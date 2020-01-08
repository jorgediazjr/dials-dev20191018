'''
Jorge A. Diaz Jr

This small program is to find spots that
are close to each other from the reflections
that are found by running dials.find_spots
and is used by dials.image_viewer
    - a checkbox allows the user to see any of the close spots

'''
import os, platform, subprocess, sys


def get_xyz_coords(result):
    """
    Parameters
    ----------
    result: dict
        holds the centroids of the experiment

    Returns
    -------
    dict:
        xyz coordinates are saved in dictionary
    """
    xyz_coords = {}
    centroids = result['xyzobs.px.value']

    for i in range(len(centroids)):
        x = centroids[i][0]
        y = centroids[i][1]
        z = centroids[i][2]
        if x in xyz_coords:
            xyz_coords[x].append([y, z])
            continue
        xyz_coords[x] = [y, z]

    return xyz_coords


def order_dictionary(dictionary):
    """
    Parameters
    ----------
    dictionary: dict
        all x and y pairs

    Returns
    -------
    ordered_dict:
        the dictionary has been ordered
    """
    import collections
    return collections.OrderedDict(sorted(dictionary.items()))


def add_index_to_pairs(ordered_pairs):
    """
    Creates a dictionary where each KEY is a integer index and
    the VALUE is a dictionary of the x,y pairs

    Parameters
    ----------
    ordered_pairs: dict
        dictionary that has been ordered

    Returns
    -------
    dict
        this dictionary has
        OUTER_KEY = integer index
        INNER_KEY = x value
        VALUE = y value
    """
    new_dict = {}
    for i, x in enumerate(ordered_pairs):
        new_dict[i] = {x: ordered_pairs[x]}
    return new_dict


def find_distance_2d(p, q):
    """
    Returns the distance between two points

    Parameters
    ----------
    p: list
        a two-value list where p[0] is the x_value and p[1] is the y_value of 2nd point

    q: list
        a two-value list where q[0] is the x_value and q[1] is the y_value of 2nd point

    Returns
    -------
    float
        the distance between points p and q
    """
    import math
    dist = math.sqrt(((p[0] - q[0])*(p[0] - q[0])) +
                     ((p[1] - q[1])*(p[1] - q[1])))
    return dist


def units_distance(p, q, distance):
    """
    Parameters
    ----------
    p: list
        first element is x1 val and second element is y1 value
    q: list
        first element is x2 val and second element is y2 value
    distance: float
        what is going to be the upper threshold to calculating distance between p and q

    Returns
    -------
    float:
        distance between p and q
    """
    a = 1600
    b = 1600
    rad_0 = 50
    rad_1 = 200
    rad_2 = 600
    rad_3 = 1000
    rad_4 = 1400
    rad_5 = 1800
    point_1 = ((p[0] - a) * (p[1] - b))
    point_2 = ((q[0] - a) * (q[1] - b))
    if (point_1 < rad_0 * rad_0) and (point_2 < rad_0 * rad_0):
        return distance
    elif((point_1 > rad_0 * rad_0) and (point_2 > rad_0 * rad_0) and
         (point_1 < rad_1 * rad_1) and (point_2 < rad_1 * rad_1)):
        return distance + 5
    elif((point_1 > rad_1 * rad_1) and (point_2 > rad_1 * rad_1) and
         (point_1 < rad_2 * rad_2) and (point_2 > rad_2 * rad_2)):
        return distance + 10
    elif((point_1 > rad_2 * rad_2) and (point_2 > rad_2 * rad_2) and
         (point_1 < rad_3 * rad_3) and (point_2 < rad_3 * rad_3)):
        return distance + 15
    elif((point_1 > rad_3 * rad_3) and (point_2 > rad_3 * rad_3) and
         (point_1 < rad_4 * rad_4) and (point_2 < rad_4 * rad_4)):
        return distance + 20
    elif((point_1 > rad_4 * rad_4) and (point_2 > rad_4 * rad_4) and
         (point_1 < rad_5 * rad_5) and (point_2 < rad_5 * rad_5)):
        return distance + 25
    elif((point_1 > rad_5 * rad_5) and (point_2 > rad_5 * rad_5)):
        return distance + 30
    else:
        return distance


def euclidean_distance(ordered_points, distance=7):
    """
    Parameters
    ----------
    ordered_points: dict
        OUTER KEY is an integer index
        INNER KEY = x value
        VALUE = y value


    Returns
    -------
    list:
        pairs of points that are close together 2D-based
    """
    close_points = []        # each element is a point that was found to be close to another point
    pairs = []
    for index in ordered_points:
        for x_coord in ordered_points[index]:
            x1 = x_coord
            y1 = ordered_points[index][x_coord][0]
            z1 = ordered_points[index][x_coord][1]
            point_a = [x1, y1, z1]
            current = 0
            while current < len(ordered_points):
                if current != index:
                    x2 = list(ordered_points[current].keys())[0]
                    y2 = ordered_points[current][x2][0]
                    z2 = ordered_points[current][x2][1]
                    point_b = [x2, y2, z2]
                    if point_a in close_points and point_b in close_points:
                        current += 1
                        continue
                    dist = find_distance_2d(point_a, point_b)
                    # new_distance = units_distance(point_a, point_b, distance)
                    # print below is helpful too see what is the avg value for distance
                    # print("dist = {}".format(dist))
                    if dist <= distance:
                        print("({:.2f}, {:.2f})\t<-\t{:.2f}\t->\t({:.2f}, {:.2f})\tWITHIN\t{}".format(point_a[0], point_a[1], dist,
                                                                                                      point_b[0], point_b[1], distance))
                        close_points.append(point_a)
                        close_points.append(point_b)
                        pairs.append([tuple(point_a), tuple(point_b), dist])
                    current += 1
                    continue
                current += 1
    return close_points, pairs


def save_spots_in_vec3(close_points):
    """
    Saves all the spots in a vec2_double object from flex module

    Parameters
    ----------
    close_points: list
        each element in this list is a list of two values, x and y

    Returns
    -------
    flex.vec2_double() object
        each element is a tuple of x,y pairs
    """
    from scitbx.array_family import flex
    close_vec3 = flex.vec3_double()

    for point in close_points:
        # NOTE: point[0] = x value | point[1] = y value | point[2] = z value
        if( (point[0] >= 1250 and point[0] <= 1276 and
             point[1] >= 1612 and point[1] <= 1614)
             or
            (point[0] >= 1539 and point[0] <= 1546 and
             point[1] >= 1603 and point[1] <= 1605)
           ):
            continue
        close_vec3.append(point)
    return close_vec3


def get_file(filename=None):
    """
    Returns either imported.expt or imported_experiments.json as filename

    Parameters
    ----------
    filename: str, optional
        user can provide which of the imported file to use or program does the searching for you

    Returns
    -------
    str
        the actual filename of the file we will be using to extract data from
    """
    if filename is None:
        if os.path.isfile("imported.expt"):
            filename = "imported.expt"
        elif os.path.isfile("imported_experiments.json"):
            filename = "imported_experiments.json"
        else:
            print("No available file ...\nMake sure you imported the experiments using dials.import ...")
    return filename


def get_base_path(filename):
    """
    Opens filename and extracts the template value, which is the path to the file used in dials.import

    Parameters
    ----------
    filename: str
        this is the name of the file that has the metadata/info about experiment

    Returns
    -------
    str
        the path to the file without the filename in the string
    """
    with open(filename, 'r') as f:
        for line in f:
            if "template" in line:
                template = str(line.split(":")[1].strip().replace("\n","").replace("\"", "").replace(",",""))
                return os.path.dirname(template)
    return "There is not template in the file {} ...".format(filename)


def get_distance_n_wavelength_cbf_version(cbf_path):
    """
    Uses a cbf file from the cbf_path to extract the wavelength and detector distance

    Parameters
    ----------
    cbf_path: str
        full path to where the cbf files for the experiments can be found

    Returns
    -------
    wavelength: float
        wavelength of experiment

    detector_distance: float
        distance from sample to detector of experiment
    """
    wavelength = -1
    detector_distance = -1
    for subdir, dirs, files in os.walk(cbf_path):
        for f in files:
            if f.endswith("cbf"):
                print("\ncbf file path: {}".format(os.path.join(subdir, f)))
                with open(os.path.join(subdir, f), 'r') as f_:
                    for line in f_:
                        if "wavelength" in line.lower():
                            wavelength = line.lower()
                        if "detector_distance" in line.lower():
                            detector_distance = line.lower()
            break
    return float(wavelength.split()[2]), float(detector_distance.split()[2])


def get_distance_n_wavelength_h5_version(h5_path):
    """
    Converts master.h5 file using eiger2cbf and extracts wavelength and detector distance

    Parameters
    ----------
    h5_path: str
        full path to where the files for the experiments can be found, not the files themselves

    Returns
    -------
    wavelength: float
        wavelength of experiment

    detector_distance: float
        distance from sample to detector of experiment
    """
    wavelength = -1
    detector_distance = -1
    h5_file = ""
    for filename in os.listdir(h5_path):
        if filename.endswith("master.h5"):
            h5_file = filename
            break
    h5_full_path = os.path.join(h5_path, h5_file)
    print("\nh5_file_path = {}".format(h5_full_path))

    cmd = "where" if platform.system() == "Windows" else "which"
    executable = "eiger2cbf"
    try: 
        exec_path = subprocess.getstatusoutput("{} {}".format(cmd, executable))[1]
        print("exec path = {}".format(exec_path))
    except: 
        print("{} executable does not exist".format(executable))
        return

    process = subprocess.Popen([executable, h5_full_path, "1", "out.cbf"],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    return get_distance_n_wavelength_cbf_version(h5_path)


def get_detector_distance_n_wavelength(filename=None):
    """
    Parameters
    ----------
    filename: str, optional
        the name of the file where the metadata/info about experiments is located

    Returns
    -------
    wavelength: float
        wavelength of experiment

    detector_distance: float
        distance from sample to detector of experiment
    """
    filename = get_file(filename)
    base_path = get_base_path(filename)

    if os.path.isdir(os.path.join(base_path, "cbf")): # we check for cbf first since it's easier
        cbf_path = os.path.join(base_path, "cbf")
        wavelength, detector_distance = get_distance_n_wavelength_cbf_version(cbf_path)
    else:
        h5_path = base_path
        wavelength, detector_distance = get_distance_n_wavelength_h5_version(h5_path)

    print("Wavelength = {} A\nDetector distance = {} mm".format(wavelength, detector_distance))


def get_reciprocal_lattice_points(experiments, reflections, closest_points):
    """
    add DOCUMENTATION later
    """
    refl_dict = {}
    for i, reflection in enumerate(reflections['xyzobs.px.value']):
        if reflection not in closest_points:
            reflections['xyzobs.px.value'][i] = (-10000000000,-10000000000,-10000000000)
            continue
        refl_dict[i] = reflection

    reflections.centroid_px_to_mm(experiments)
    reflections.map_centroids_to_reciprocal_space(experiments)

    rlp_dict = {}
    for i, val in enumerate(reflections['rlp']):
        if val != (-0.7684894267425115, 0.3341246142735701, -1.288443906247419):
            rlp_dict[i] = val

    print("\n\n")
    for key in rlp_dict:
        print("{}\t=>\trlp\t=>\t{}".format(refl_dict[key], rlp_dict[key]))
    return reflections, refl_dict, rlp_dict


def find_distance_3d(p, q):
    """
    DOCUMENTATION later
    """
    import math
    dist = math.sqrt( ( (p[0] - q[0]) * (p[0] - q[0]) ) +
                      ( (p[1] - q[1]) * (p[1] - q[1]) ) +
                      ( (p[2] - q[2]) * (p[2] - q[2]) ) )
    return dist


def euclidean_distance_for_reciprocal_lattice_pts(reflections):
    """
    DOCUMENTATION later
    """
    print("\n\n")
    good_rlp = []
    for i, rlp in enumerate(reflections['rlp']):
        if rlp != (-0.7684894267425115, 0.3341246142735701, -1.288443906247419):
            good_rlp.append(rlp)
    
    closest_rlps = []
    for i, rlp in enumerate(good_rlp):
        index = 0
        p1 = rlp
        while index < len(good_rlp):
            if index != i:
                p2 = good_rlp[index]
                dist = find_distance_3d(p1, p2)
                if dist <= 0.00005:
                    print("{}\t<=\t{:.9f}\t=>\t{}".format(p1, dist, p2))
                    closest_rlps.append(p1)
                    closest_rlps.append(p2)
            index += 1

    return closest_rlps


def find_match_refl_rlp(close_spots, closest_rlps, refl_dict, rlp_dict):
    """
    DOCUMENTATION later
    """
    new_close_spots = []
    for key in rlp_dict:
        if rlp_dict[key] in closest_rlps:
            new_close_spots.append(refl_dict[key])


def find_rlp_pairs_from_refl(pairs, refl_dict, rlp_dict):
    print("\n\nfind_rlp_pairs_from_refl...\n")
    rlp_pairs = []
    for pair in pairs:
        p1 = pair[0]
        p2 = pair[1]
        q1 = 0
        q2 = 0
        for key in refl_dict:
            if p1 == refl_dict[key]:
                print("{}".format(key))
                q1 = rlp_dict[key]
            if p2 == refl_dict[key]:
                q2 = rlp_dict[key]
        rlp_pairs.append([tuple(q1), tuple(q2), find_distance_3d(q1, q2)])

    for pair in rlp_pairs:
        print("RLP pair => {}".format(pair))



def main(reflections, dist=None):
    """
    Parameters
    ----------
    reflections: dict
        the KEY in the dictionary is xyzobs.px.value and
        the VALUE is a list of all reflections in tuple format

    dist: integer or float, optional
        the desired distance user wants program to use

    Returns
    -------
    flex.vec2_double() object
        each element in this object is a tuple of size 2 => (x_val, y_val)
    """
    print("\n")
    xyz_coords = get_xyz_coords(reflections)
    ordered_points = order_dictionary(xyz_coords)
    ordered_points = add_index_to_pairs(ordered_points)
    
    if dist is None: # user did not provide distance
        close_points, pairs = euclidean_distance(ordered_points)
    else:
        close_points, pairs = euclidean_distance(ordered_points, dist)

    for i, p in enumerate(pairs):
        print("pair {} : {}".format(i, p))

    # get_detector_distance_n_wavelength("imported.expt")
    close_vec3 = save_spots_in_vec3(close_points)

    print("Number of spots: {}/{}".format(len(close_vec3), len(ordered_points)))

    return close_vec3, pairs