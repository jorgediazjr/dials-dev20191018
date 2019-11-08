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

    print("JAD7: centroids = {}".format(centroids))
    print("JAD7: intensities = {}".format(intensities))
    print("x\t\ty\t\tz\t\tintensities")
    
    '''
    0   =   1343.04     1007.89     0.98        5885.0
    1   =   1695.48     1257.23     0.99        1100.0
    2   =   1676.59     1266.09     0.85        817.0   
    '''

    print("JAD7: result.nrows() = {}".format(result.nrows()))
    print("JAD7: result.ncols() = {}".format(result.ncols()))
    print("JAD7: len(centroids) = {}".format(len(centroids)))
    print("JAD7: len(intensities) = {}".format(len(intensities)))

    for i in range(len(centroids)):
        print("{}\t=\t{:.2f}\t\t{:.2f}\t\t{:.2f}\t\t{}".format(i,
                                                               centroids[i][0],
                                                               centroids[i][1],
                                                               centroids[i][2],
                                                               intensities[i])
        )

    result['xyzobs.px.value'] = centroids
    #result['intensity.sum.value'] = intensities
    #result.as_file("strong.refl")
    
