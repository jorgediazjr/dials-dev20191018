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
    print("~~~~~~~~~~~~CLOSE_SPOTS FILE~~~~~~~~~~~~")
    reflections.as_pickle("P_TEST.pickle")
    result = flex.reflection_table.from_file("P_TEST.pickle")
    print("JAD7: find_spots - result => {}".format(result))
    print("JAD7: find_spots - dir(result) => {}".format(dir(result)))
    print("JAD7: find_spots - result.keys() => {}".format(result.keys()))
    for key in result.keys():
        print("JAD7: find_spots - result.keys()[key] => {}".format(key))


