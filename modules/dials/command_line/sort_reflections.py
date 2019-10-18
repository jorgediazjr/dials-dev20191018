# LIBTBX_SET_DISPATCHER_NAME dev.dials.sort_reflections

from __future__ import absolute_import, division, print_function

import dials.util
from dials.array_family import flex

help_message = """

Utility script to sort reflection tables by the values in a column.

Example::

  dev.dials.sort_reflections key=miller_index output=sorted.refl
"""


class Sort(object):
    """A class for running the script."""

    def __init__(self):
        """Initialise the script."""
        from dials.util.options import OptionParser
        from libtbx.phil import parse

        phil_scope = parse(
            """

            key = 'miller_index'
                .type = str
                .help = "The chosen sort key. This should be a column of "
                        "the reflection table."

            reverse = False
                .type = bool
                .help = "Reverse the sort direction"

            output = sorted.refl
                .type = str
                .help = "The output reflection filename"
            """
        )

        usage = "dev.dials.sort_reflections [options] observations.refl"

        # Initialise the base class
        self.parser = OptionParser(
            usage=usage, phil=phil_scope, read_reflections=True, epilog=help_message
        )

    @staticmethod
    def sort_permutation(column, reverse=False):
        indices = flex.size_t_range(len(column))
        perm = sorted(indices, key=lambda k: column[k], reverse=reverse)
        return flex.size_t(perm)

    def run(self):
        """Execute the script."""
        from dials.util.options import flatten_reflections

        # Parse the command line
        params, options = self.parser.parse_args(show_diff_phil=True)
        reflections = flatten_reflections(params.input.reflections)
        if not reflections:
            self.parser.print_help()
            return
        if len(reflections) != 1:
            raise dials.util.Sorry("exactly 1 reflection table must be specified")
        reflections = reflections[0]

        # Check the key is valid
        assert params.key in reflections

        # Sort the reflections
        print("Sorting by %s with reverse=%r" % (params.key, params.reverse))
        perm = self.sort_permutation(reflections[params.key], params.reverse)
        reflections = reflections.select(perm)

        if options.verbose > 0:
            print("Head of sorted list " + params.key + ":")
            n = min(len(reflections), 10)
            for i in range(n):
                print(reflections[i][params.key])

        # Save sorted reflections to file
        if params.output:
            print("Saving reflections to {}".format(params.output))
            reflections.as_file(params.output)


if __name__ == "__main__":
    with dials.util.show_mail_on_error():
        script = Sort()
        script.run()
