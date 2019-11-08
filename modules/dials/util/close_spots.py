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

def find_close_spots(result):

    pass


def main(reflections):
    print("~~~~~~~~~~~~CLOSE_SPOTS FILE~~~~~~~~~~~~")
    result = read_in_file()
    x_y_pairs = get_x_y_pairs(result)

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
    
