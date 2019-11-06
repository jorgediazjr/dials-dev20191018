'''
Jorge Diaz

This small program is to find spots that
are close to each other from the reflections
that are found by running dials.find_spots

'''


def read_in_file():
    from dials.array_family import flex
    return flex.reflection_table.from_file("strong.refl")


def find_close_spots():
    pass


def main(reflections):
    print("~~~~~~~~~~~~CLOSE_SPOTS FILE~~~~~~~~~~~~")
    # reflections.as_pickle(".pickle")
    result = read_in_file()

    print("JAD7: close_spots - result => {}".format(result))
    for item in dir(result):
        print("JAD7: dir(result) = {}".format(item))

    print("JAD7: close_spots - result.keys() => {}".format(result.keys()))
    for key, value in result.items():
        print("JAD7: close_spots - result.keys()[key] => {} => value: {}\n".format(key, value))
    
    print("JAD7: close_spots - result[intensity.sum.value] = {}".format(result['intensity.sum.value']))
    for item in dir(result['intensity.sum.value']):
        print("JAD7: close_spots - dir(result[intensity.sum.value]) = {}".format(item))
    

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

    print("JAD7: result[xyzobs.px.value] = {}".format(result['xyzobs.px.value']))
    print("JAD7: centroids = {}".format(centroids))
    print("x\t\ty\t\tz\t\tintensities")
    # del centroids[0]
    
    for i in range(len(centroids)):
        print("{}\t=\t{:.2f}\t\t{:.2f}\t\t{:.2f}\t\t{}".format(i,
                                                               centroids[i][0],
                                                               centroids[i][1],
                                                               centroids[i][2],
                                                               intensities[i])
        )
    print("JAD7: intensities = {}".format(intensities))
