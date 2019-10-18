from __future__ import absolute_import, division, print_function
from xfel.merging.application.errors.error_modifier_sr import error_modifier_sr
from xfel.merging.application.errors.error_modifier_ha14 import error_modifier_ha14
from xfel.merging.application.worker import factory as factory_base

class factory(factory_base):
  '''Factory class for modifying errors of measured intensities'''
  @staticmethod
  def from_parameters(params, additional_info=[], mpi_helper=None, mpi_logger=None):
    assert len(additional_info) > 0
    assert additional_info[0] in ["premerge", "merge"]
    if additional_info[0] == "premerge":
      if params.merging.error.model == "ha14":
        return [error_modifier_ha14(params, mpi_helper, mpi_logger)]
    elif additional_info[0] == "merge":
      if params.merging.error.model == "errors_from_sample_residuals":
        return [error_modifier_sr(params, mpi_helper, mpi_logger)]
    return []
