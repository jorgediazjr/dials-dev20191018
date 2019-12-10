'''
Jorge A. Diaz Jr

This small program is to find spots that
are close to each other from the reflections
that are found by running dials.find_spots
and is used by dials.image_viewer
    - a checkbox allows the user to see any of the close spots

'''


def read_in_file():
    from dials.array_family import flex
    return flex.reflection_table.from_file("strong.refl")


def get_xyz_coords(result):
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
    import collections
    return collections.OrderedDict(sorted(dictionary.items()))


def add_index_to_pairs(ordered_pairs):
    new_dict = {}
    for i, x in enumerate(ordered_pairs):
        new_dict[i] = {x: ordered_pairs[x]}
    return new_dict


def find_distance(p, q):
    import math
    dist = math.sqrt(((p[0] - q[0])*(p[0] - q[0])) +
                     ((p[1] - q[1])*(p[1] - q[1])))
    return dist


def units_distance(p, q, distance):
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
    close_points = []        # these are pairs of points that are close together
    closest_points = dict()  # this has x,y values that are closest
    midpoints = dict()      # the midpoints between the close pairs
    for index in ordered_points:
        for x_coord in ordered_points[index]:
            x1 = x_coord
            y1 = ordered_points[index][x_coord][0]
            point_a = [x1, y1]
            current = 0
            while current < len(ordered_points):
                if current != index:
                    x2 = list(ordered_points[current].keys())[0]
                    y2 = ordered_points[current][x2][0]
                    point_b = [x2, y2]
                    if point_a in close_points and point_b in close_points:
                        current += 1
                        continue
                    dist = find_distance(point_a, point_b)
                    # new_distance = units_distance(point_a, point_b, distance)
                    # print below is helpful too see what is the avg value for distance
                    # print("dist = {}".format(dist))
                    if dist <= distance:
                        pair = [point_a, point_b]
                        print("({:.2f}, {:.2f})\t<-\t{:.2f}\t->\t({:.2f}, {:.2f})\tWITHIN\t{}".format(point_a[0], point_a[1], dist,
                                                                                                      point_b[0], point_b[1], distance))
                        close_points.append(point_a)
                        close_points.append(point_b)
                        # close_points.append(pair)
                        closest_points[point_b[0]] = [point_b[1]]
                        # save the midpoints of the close pairs
                        midpoint_x = (point_a[0] + point_b[0]) / 2
                        midpoint_y = (point_a[1] + point_b[1]) / 2
                        midpoints[midpoint_x] = midpoint_y
                    current += 1
                    continue
                current += 1
    return close_points, midpoints, closest_points


def flatten_closest_points(close_points):
        points = []
        final_points = []
        for i in close_points:
            points.append(i[0])
            points.append(i[1])

        for i in points:
            if i in final_points:
                continue
            final_points.append(i)

        return final_points


def save_spots_in_vec2(close_points):
    from scitbx.array_family import flex
    close_vec2 = flex.vec2_double()

    # close_points = flatten_closest_points(close_points)

    for point in close_points:   # point[0] = x | point[1] = y
        if( (point[0] >= 1250 and point[0] <= 1275 and
             point[1] >= 1612.5 and point[1] <= 1613.5)
             or
            (point[0] >= 1539 and point[0] <= 1546 and
             point[1] >= 1603 and point[1] <= 1605)
           ):
            continue
        close_vec2.append(point)
    return close_vec2


def make_vec2_same_num_rows_for_reflections(close_vec2, reflections):
    close_vec_len = len(close_vec2)
    reflections_len = len(reflections)
    while close_vec_len < reflections_len:
        close_vec2.append([-float(0), -float(0)])
        close_vec_len += 1
    print("len(reflections) = {} and len(close_vec2) = {}".format(len(reflections), len(close_vec2)))
    return close_vec2


def get_detector_distance(beam_centre):
    pass


def get_detector_wavelength(filename=None):
    import os.path
    if filename is None:
        if os.path.isfile("imported.expt"):
            filename = "imported.expt"
        elif os.path.isfile("imported_experiments.json"):
            filename = "imported_experiments.json"
        else:
            print("No available file ...\nMake sure you imported the experiments using dials.import ...")
    with open(filename, 'r') as f:
        for line in f:
            if "wavelength" in line:
                print("Wavelength in this line --> {}".format(line))
                return round(float(line.split(":")[1].replace("\n", "")), 17)
    return "Wavelength not found in file: {}".format(filename)


def main(reflections, beam_x, beam_y, dist=None):
    xyz_coords = get_xyz_coords(reflections)
    ordered_points = order_dictionary(xyz_coords)
    ordered_points = add_index_to_pairs(ordered_points)
    
    if dist is None:
        close_points, midpoints, closest_points = euclidean_distance(ordered_points)
    else:
        close_points, midpoints, closest_points = euclidean_distance(ordered_points, dist)

    # closest_points = order_dictionary(closest_points)
    beam_centre = (beam_x, beam_y)
    wavelength = get_detector_wavelength("imported.expt")
    close_vec2 = save_spots_in_vec2(close_points)

    print("Beam centre is ({}, {})".format(beam_centre[0], beam_centre[1]))
    print("wavelength is {} of type {}".format(wavelength, type(wavelength)))
    print("Number of spots: {}/{}".format(len(close_vec2), len(ordered_points)))

    return close_vec2

    '''
    KEYS FROM READING IN PICKLE REFLECTIONS
        bbox
        flags
        id
        intensity.sum.value
        intensity.sum.variance
        panel
        shoebox
        xyzobs.px.value
        xyzobs.px.variance
    '''
    
