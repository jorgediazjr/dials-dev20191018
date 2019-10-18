"""
Phil scope of options for scaling.
"""
from __future__ import absolute_import, division, print_function
import iotbx.phil

phil_scope = iotbx.phil.parse(
    """
  overwrite_existing_models = False
    .type = bool
    .help = "If True, create new scaling models for all datasets"
    .expert_level = 0
  reflection_selection {
    method = *auto quasi_random intensity_ranges use_all random
      .type = choice
      .help = "Method to use when choosing a reflection subset for scaling model"
              "minimisation. auto (default) will choose use_all for small datasets"
              "and quasi_random for large datasets, and will try to optimise the"
              "quasi_random algorithm parameters. Manually selecting quasi-random"
              "will use the reflection_selection.quasi_random parameters to"
              "attempt to choose reflection groups that have a good connectedness"
              "across reciprocal space, for all resolutions. intensity_ranges"
              "uses the E2_range, Isigma_range and d_range options to choose a"
              "subset of reflections. use_all uses all suitable reflections for"
              "model minimisation, which may be slow for large datasets."
    quasi_random {
      min_per_area = 100
        .type = ints
        .help = "Numbers of reflections for each of the 12 volumes in"
                "reciprocal space at a given resolution."
        .expert_level = 2
      n_resolution_bins = 20
        .type = ints
        .help = "Number of resolution bins for quasi random sampling."
        .expert_level = 2
      multi_dataset {
        min_per_dataset = 500
          .type = int
          .help = "Minimum number of cross-dataset connected reflections in"
                  "each dataset."
        Isigma_cutoff = 1.0
          .type = float
          .help = "Minimum average I/sigma of reflection groups to use when"
                  "selecting cross-dataset connected reflections."
        min_multiplicity = 2
          .type = int
          .help = "Minimum multiplicity of cross-dataset connected reflections"
                  "for reflections used during minimisation."
      }
    }
    n_random = 50000
      .type = int
      .help = "Number of randomly chosen reflections to use"
    best_unit_cell = None
      .type = floats(size=6)
      .help = "Best unit cell value, to use when performing resolution cutting
               and merging statistics. If None, the median unit cell will be used."
    E2_range = 0.8, 5.0
      .type = floats(size=2)
      .help = "Minimum and maximum normalised E^2 value to used to select a
              subset of reflections for minimising the scaling model."
      .expert_level = 1
    Isigma_range = -5.0, 0.0
      .type = floats(size=2)
      .help = "Minimum and maximum I/sigma values used to subset of reflections
              to determine the scaling model. Here a value of 0.0 for the max
              means no limit applied."
      .expert_level = 1
    d_range = None
      .type = floats(size=2)
      .help = "Minimum and maximum - values used to subset of reflections
              to determine the scaling model."
      .expert_level = 1
    min_partiality = 0.95
      .type = float
      .help = "Minimum partiality to use when selecting reflections to use
               to determine the scaling model and error model."
      .expert_level = 2
    intensity_choice = profile sum *combine
      .type = choice
      .help = "Option to choose from profile fitted or summation intensities, or
               an optimised combination of profile/sum."
      .expert_level = 1
    combine.Imid = None
      .type = floats
      .help = "A list of values to try for the midpoint, for profile/sum combination
               calculation: the value with the lowest Rmeas will be chosen.
               0 and 1 are special values that can be supplied to include profile
               and sum respectively in the comparison."
      .expert_level = 2
    combine.joint_analysis = True
      .type = bool
      .help = "Option of whether to do intensity combination optimisation
              separately (i.e. different Imid per dataset) or joint for
              multiple datasets"
      .expert_level = 2
  }
  weighting {
    weighting_scheme = *invvar
      .type = choice
      .help = "Weighting scheme used during Ih calculation. Weighting schemes
              other than invvar and unity may trigger iterative reweighting
              during minimisation, which may be unstable for certain minimisation
              engines (LBFGS)."
      .expert_level = 2
    error_model {
      error_model = *basic None
        .type = choice
        .help = "The error model to use."
        .expert_level = 1
      min_Ih = 25.0
        .type = float
        .help = "Reflections with expected intensity above this value are to."
                "be used in error model minimisation."
        .expert_level = 2
      n_bins = 10
        .type = int
        .help = "The number of intensity bins to use for the error model optimisation."
        .expert_level = 2
    }
  }
  cut_data {
    d_min = None
      .type = float
      .help = "Option to apply a high resolution cutoff for the dataset (i.e.
               the chosen reflections have d > d_min)."
      .expert_level = 1
    d_max = None
      .type = float
      .help = "Option to apply a low resolution cutoff for the dataset (i.e.
               the chosen reflections have d < d_max)."
      .expert_level = 1
    partiality_cutoff = 0.4
      .type = float
      .help = "Value below which reflections are removed from the dataset due
               to low partiality."
      .expert_level = 1
  }
  scaling_options {
    check_consistent_indexing = False
      .type = bool
      .help = "If True, run dials.cosym on all data in the data preparation"
              "step, to ensure consistent indexing."
    target_cycle = True
      .type = bool
      .help = "Option to turn of initial round of targeted scaling
               if some datasets are already scaled."
      .expert_level = 2
    only_target = False
      .type = bool
      .help = "Option to only do targeted scaling if some datasets
               are already scaled."
      .expert_level = 2
    only_save_targeted = True
      .type = bool
      .help = "If only_target is true, this option to change whether the dataset
              that is being scaled will be saved on its own, or combined with the
              already scaled dataset."
      .expert_level = 2
    target_model = None
      .type = path
      .help = "Path to cif file to use to calculate target intensities for
              scaling."
      .expert_level = 2
    target_mtz = None
      .type = path
      .help = "Path to merged mtz file to use as a target for scaling."
      .expert_level = 2
    nproc = 1
      .type = int(value_min=1)
      .help = "Number of blocks to divide the data into for minimisation.
              This also sets the number of processes to use if the option is
              available."
      .expert_level = 2
    use_free_set = False
      .type = bool
      .help = "Option to use a free set during scaling to check for overbiasing.
              This free set is used to calculate an RMSD, which is shown alongisde
              the 'working' RMSD during refinement, but is not currently used
              to terminate refinement or make any choices on the model."
      .expert_level = 2
    free_set_percentage = 10.0
      .type = float
      .help = "Percentage of symmetry equivalent groups to use for the free set,
              if use_free_set is True."
      .expert_level = 2
    free_set_offset = 0
      .type = int
      .help = "Offset for choosing unique groups for the free set from the whole
               set of unique groups."
      .expert_level = 2
    space_group = None
      .type = str
      .help = "Option to specify space group for scaling (deprecated)"
      .expert_level = 1
    full_matrix = True
      .type = bool
      .help = "Option to turn off GN/LM refinement round used to determine
               error estimates on scale factors."
      .expert_level = 2
    outlier_rejection = *standard simple
      .type = choice
      .help = "Choice of outlier rejection routine. Standard may take a
        significant amount of time to run for large datasets or high
        multiplicities, whereas simple should be quick for these datasets."
      .expert_level = 1
    outlier_zmax = 6.0
      .type = float(value_min=3.0)
      .help = "Cutoff z-score value for identifying outliers based on their
               normalised deviation within the group of equivalent reflections"
      .expert_level = 1
  }
  """
)
