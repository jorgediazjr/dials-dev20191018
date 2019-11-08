'''
Jorge Diaz

This small program is to find spots that
are close to each other from the reflections
that are found by running dials.find_spots

'''


def read_in_file():
    from dials.array_family import flex
    return flex.reflection_table.from_file("strong.refl")


def get_x_y_pairs(result):
    print("JAD7: In get_xy_pairs()")
    xy_pairs = {}
    centroids = result['xyzobs.px.value']

    for i in range(len(centroids)):
        x = centroids[i][0]
        y = centroids[i][1]
        if x in xy_pairs:
            xy_pairs[x].append(y)
            continue
        xy_pairs[x] = [y]

    return xy_pairs


def order_xy_pairs(xy_pairs):
    import collections
    return collections.OrderedDict(sorted(xy_pairs.items()))


def add_index_to_pairs(ordered_pairs):
    new_dict = {}
    for i, x in enumerate(ordered_pairs):
        new_dict[i] = {x: ordered_pairs[x]}
    return new_dict


def euclidean_distance(ordered_pairs, distance=0.5):
    close_pairs = []        # these are pairs of points that are close together
    closest_pairs = dict()  # this has x,y values that are closest
    midpoints = dict()      # the midpoints between the close pairs
    for index in ordered_pairs:
        for x_coord in ordered_pairs[index]:
            for y_coord in ordered_pairs[index][x_coord]:
                x1 = x_coord
                y1 = y_coord
                point_a = [x1, y1]
                current = 0
                while current < len(ordered_pairs):
                    if current != index:
                        x2 = list(ordered_pairs[current].keys())[0]
                        for _y in ordered_pairs[current][x2]:
                            y2 = _y
                            point_b = [x2, y2]
                            dist = find_distance(point_a, point_b)
                            distance = units_distance(point_a, point_b)
                            if dist <= distance:
                                pair = [point_a, point_b]
                                print("{}\t<-- {:.2f} -->\t{}\tWITHIN {}".format(point_a, dist, point_b, distance))
                                close_pairs.append(pair)
                                closest_pairs[point_b[0]] = [point_b[1]]
                                # save the midpoints of the close pairs
                                midpoint_x = (point_a[0] + point_b[0]) / 2
                                midpoint_y = (point_a[1] + point_b[1]) / 2
                                midpoints[midpoint_x] = midpoint_y
                        current += 1
                    else:
                        current += 1
    return close_pairs, midpoints, closest_pairs

def find_close_spots(x_y_pairs):

    pass


def main(reflections):
    print("~~~~~~~~~~~~CLOSE_SPOTS FILE~~~~~~~~~~~~")
    result = read_in_file()
    x_y_pairs = get_x_y_pairs(result)
    print("JAD7: Finished get_xy_pairs()")

    ordered_pairs = order_xy_pairs(x_y_pairs)
    ordered_pairs = add_index_to_pairs(ordered_pairs)
    close_pairs, midpoints, closest_pairs = euclidean_distance(ordered_pairs)








    print("JAD7: close_spots - result => {}".format(result))
    for item in dir(result):
        print("JAD7: dir(result) = {}".format(item))

    print("JAD7: close_spots - result.keys() => {}".format(result.keys()))
    for key, value in result.items():
        print("JAD7: close_spots - result.keys()[key] => {} => value: {}\n".format(key, value))

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
    centroids = result['xyzobs.px.value']
    intensities = result['intensity.sum.value']

    print("JAD7: centroids = {}".format(centroids))
    print("JAD7: intensities = {}".format(intensities))
    print("JAD7: result.nrows() = {}".format(result.nrows()))
    print("JAD7: result.ncols() = {}".format(result.ncols()))
    print("JAD7: len(centroids) = {}".format(len(centroids)))
    print("JAD7: len(intensities) = {}".format(len(intensities)))

    print("x\t\ty\t\tz\t\tintensities")
    for i in range(len(centroids)):
        print("{}\t=\t{:.2f}\t\t{:.2f}\t\t{:.2f}\t\t{}".format(i,
                                                               centroids[i][0],
                                                               centroids[i][1],
                                                               centroids[i][2],
                                                               intensities[i])
        )
    
