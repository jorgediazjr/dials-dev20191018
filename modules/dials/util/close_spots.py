'''
Jorge Diaz

This small program is to find spots that
are close to each other from the reflections
that are found by running dials.find_spots

'''


def read_in_file():
    pass


def find_close_spots():
    pass


def main(reflections):
    from dials.array_family import flex
    print("~~~~~~~~~~~~CLOSE_SPOTS FILE~~~~~~~~~~~~")
    reflections.as_pickle("P_TEST.pickle")
    result = flex.reflection_table.from_file("P_TEST.pickle")

    print("JAD7: close_spots - result => {}".format(result))
    print("JAD7: close_spots - dir(result) => {}".format(dir(result)))
    print("JAD7: close_spots - result.keys() => {}".format(result.keys()))
    for key in result.keys():
        print("JAD7: close_spots - result.keys()[key] => {}".format(key))
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
    print("JAD7: close_spots ==> result[bbox] = {}".format(result['bbox']))
    print("JAD7: close_spots ==> dir(result[bbox]) = {}".format(dir(result['bbox'])))
    print("JAD7: close_spots ==> result[bbox].__dict__ = {}".format(result['bbox'].__dict__))
    print("JAD7: close_spots ==> result[bbox].__doc__ = {}".format(result['bbox'].__doc__))
    print("JAD7: close_spots ==> result[bbox].as_int() = {}".format(result['bbox'].as_int()))
    print("JAD7: close_spots ==> dir(result[bbox].as_int()) = {}".format(dir(result['bbox'].as_int())))
    print("JAD7: close_spots ==> dir(result[bbox].as_int().as_numpy_array()) = {}".format(dir(result['bbox'].as_int().as_numpy_array())))
    print("JAD7: close_spots ==> result[bbox].as_int().as_numpy_array() = {}".format(result['bbox'].as_int().as_numpy_array()))

    #print("JAD7: find_spots - result.flags() => {}".format(result.flags()))
    #print("JAD7: find_spots - dict(result) => {}".format(dict(result)))
    #print("JAD7: find_spots - result[xyzobs.px.value] => {}".format(result['xyzobs.px.value']))
    #print("JAD7: find_spots - dir(result[xyzobs.px.value]) => {}".format(dir(result['xyzobs.px.value'])))
    #print("JAD7: find_spots - str(result[xyzobs.px.value]) => {}".format(str(result['xyzobs.px.value'])))
    #print("JAD7: find_spots - dict(result[xyzobs.px.value][0]) => {}".format(dict(result['xyzobs.px.value'][0])))


