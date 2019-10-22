# DIALS_ENABLE_COMMAND_LINE_COMPLETION

from __future__ import absolute_import, division, print_function

import logging

from dials.util import show_mail_on_error

logger = logging.getLogger("dials.command_line.find_spots")

help_message = """

This program tries to find strong spots on a sequence of images. The program can
be called with either a "models.expt" file or a sequence of image files (see
help for dials.import for more information about how images are imported). Spot
finding will be done against each logically grouped set of images given. Strong
pixels will be found on each image and spots will be formed from connected
components. In the case of rotation images, connected component labelling will
be done in 3D.

Once a set of spots have been found, their centroids and intensities will be
calculated. They will then be filtered according to the particular preferences
of the user. The output will be a file (strong.refl) containing a list of spot
centroids and intensities which can be used in the dials.index program. To view
a list of parameters for spot finding use the --show-config option.

Examples::

  dials.find_spots image1.cbf

  dials.find_spots imager_00*.cbf

  dials.find_spots models.expt

  dials.find_spots models.expt output.reflections=strong.refl
"""

# Set the phil scope
from libtbx.phil import parse

phil_scope = parse(
    """

  output {
    reflections = 'strong.refl'
      .type = str
      .help = "The output filename"

    shoeboxes = True
      .type = bool
      .help = "Save the raw pixel values inside the reflection shoeboxes."

    experiments = None
      .type = str
      .help = "Save the modified experiments."
              "(usually only modified with hot pixel mask)"

    log = 'dials.find_spots.log'
      .type = str
      .help = "The log filename"

    closespots = False
      .type = bool
      .help = "Refines the resulting reflections by checking close spots."
  }

  per_image_statistics = False
    .type = bool
    .help = "Whether or not to print a table of per-image statistics."

  include scope dials.algorithms.spot_finding.factory.phil_scope
""",
    process_includes=True,
)


class Script(object):
    """A class for running the script."""

    def __init__(self, phil=phil_scope):
        """Initialise the script."""
        from dials.util.options import OptionParser
        import libtbx.load_env

        # The script usage
        usage = (
            "usage: %s [options] [param.phil] "
            "{models.expt | image1.file [image2.file ...]}" % libtbx.env.dispatcher_name
        )

        # Initialise the base class
        self.parser = OptionParser(
            usage=usage,
            phil=phil,
            epilog=help_message,
            read_experiments_from_images=True,
            read_experiments=True,
        )

    def run(self, args=None):
        """Execute the script."""
        from dials.array_family import flex
        from dials.util.options import flatten_experiments
        from time import time
        from dials.util import log

        start_time = time()

        # Parse the command line
        params, options = self.parser.parse_args(args=args, show_diff_phil=False)

        if __name__ == "__main__":
            # Configure the logging
            log.config(verbosity=options.verbose, logfile=params.output.log)

        from dials.util.version import dials_version

        logger.info(dials_version())

        # Log the diff phil
        diff_phil = self.parser.diff_phil.as_str()
        if diff_phil != "":
            logger.info("The following parameters have been modified:\n")
            logger.info(diff_phil)

        # Ensure we have a data block
        experiments = flatten_experiments(params.input.experiments)
        if len(experiments) == 0:
            self.parser.print_help()
            return

        # Loop through all the imagesets and find the strong spots
        reflections = flex.reflection_table.from_observations(experiments, params)

        # reflections.as_h5("TEST")
        # flex.reflection_table.from_h5("TEST")
        # reflections.as_pickle("P_TEST.pickle")
        # result = flex.reflection_table.from_file("P_TEST.pickle")

        reflections.as_h5("H5_test.h5")
        res = flex.reflection_table.from_h5("H5_test.h5")
        print("JAD7: res from h5 => {}".format(res))

        print("JAD7: find_spots - result => {}".format(result))
        print("JAD7: find_spots - dir(result) => {}".format(dir(result)))
        print("JAD7: find_spots - result.keys() => {}".format(result.keys()))
        for key in result.keys():
            print("JAD7: find_spots - result.keys()[key] => {}".format(key))
        print("JAD7: find_spots - result.flags() => {}".format(result.flags()))
        print("JAD7: find_spots - dict(result) => {}".format(dict(result)))
        print("JAD7: find_spots - result[xyzobs.px.value] => {}".format(result['xyzobs.px.value']))
        print("JAD7: find_spots - dir(result[xyzobs.px.value]) => {}".format(dir(result['xyzobs.px.value'])))
        print("JAD7: find_spots - str(result[xyzobs.px.value]) => {}".format(str(result['xyzobs.px.value'])))
        #print("JAD7: find_spots - dict(result[xyzobs.px.value][0]) => {}".format(dict(result['xyzobs.px.value'][0])))

        # Add n_signal column - before deleting shoeboxes
        from dials.algorithms.shoebox import MaskCode

        good = MaskCode.Foreground | MaskCode.Valid
        reflections["n_signal"] = reflections["shoebox"].count_mask_values(good)

        # Delete the shoeboxes
        if not params.output.shoeboxes:
            del reflections["shoebox"]

        # ascii spot count per image plot
        from dials.util.ascii_art import spot_counts_per_image_plot

        for i, experiment in enumerate(experiments):
            ascii_plot = spot_counts_per_image_plot(
                reflections.select(reflections["id"] == i)
            )
            if len(ascii_plot):
                logger.info("\nHistogram of per-image spot count for imageset %i:" % i)
                logger.info(ascii_plot)

        # Save the reflections to file
        logger.info("\n" + "-" * 80)
        reflections.as_file(params.output.reflections)
        logger.info(
            "Saved {} reflections to {}".format(
                len(reflections), params.output.reflections
            )
        )

        # Save the experiments
        if params.output.experiments:
            logger.info("Saving experiments to {}".format(params.output.experiments))
            experiments.as_file(params.output.experiments)

        # Print some per image statistics
        if params.per_image_statistics:
            from dials.algorithms.spot_finding import per_image_analysis
            from six.moves import cStringIO as StringIO

            s = StringIO()
            for i, experiment in enumerate(experiments):
                print("Number of centroids per image for imageset %i:" % i, file=s)
                imageset = experiment.imageset
                stats = per_image_analysis.stats_imageset(
                    imageset,
                    reflections.select(reflections["id"] == i),
                    resolution_analysis=False,
                )
                per_image_analysis.print_table(stats, out=s)
            logger.info(s.getvalue())

        # Print the time
        logger.info("Time Taken: %f" % (time() - start_time))

        if params.output.experiments:
            return experiments, reflections
        else:
            return reflections


if __name__ == "__main__":
    with show_mail_on_error():
        script = Script()
        script.run()
