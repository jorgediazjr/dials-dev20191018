from __future__ import absolute_import, division, print_function

import os

import procrunner
import pytest
from cctbx import sgtbx
from dxtbx.serialize import load

from dials.array_family import flex
from dials.algorithms.symmetry.cosym._generate_test_data import (
    generate_experiments_reflections,
)


def test_symmetry(dials_regression, tmpdir):
    """Simple test to check that dials.symmetry completes"""

    result = procrunner.run(
        [
            "dials.symmetry",
            os.path.join(dials_regression, "xia2-28", "20_integrated_experiments.json"),
            os.path.join(dials_regression, "xia2-28", "20_integrated.pickle"),
            os.path.join(dials_regression, "xia2-28", "25_integrated_experiments.json"),
            os.path.join(dials_regression, "xia2-28", "25_integrated.pickle"),
        ],
        working_directory=tmpdir,
    )
    assert not result.returncode and not result.stderr
    assert tmpdir.join("symmetrized.refl").check()
    assert tmpdir.join("symmetrized.expt").check()


def test_symmetry_basis_changes_for_C2(tmpdir):
    """Test the correctness of change of basis operations in dials.symmetry

    Supply the unit cell of beta-lactamase, which triggers a change of
    basis from input to minimum during symmetry analysis."""
    os.chdir(tmpdir.strpath)
    unit_cell = (53.173, 61.245, 69.292, 90.0, 93.04675, 90.0)
    space_group = sgtbx.space_group_info("C 2").group()
    experiments, reflections, _ = generate_experiments_reflections(
        space_group=space_group,
        unit_cell=unit_cell,
        sample_size=1,
        map_to_minimum=False,
    )
    experiments.as_json("tmp.expt")
    expt_file = tmpdir.join("tmp.expt").strpath
    joint_table = flex.reflection_table()
    for r in reflections:
        joint_table.extend(r)
    joint_table.as_pickle("tmp.refl")
    refl_file = tmpdir.join("tmp.refl").strpath

    command = ["dials.symmetry", expt_file, refl_file]
    result = procrunner.run(command, working_directory=tmpdir.strpath)
    assert not result.returncode and not result.stderr
    assert tmpdir.join("symmetrized.refl").check(file=1)
    assert tmpdir.join("symmetrized.expt").check(file=1)

    expts = load.experiment_list(
        tmpdir.join("symmetrized.expt").strpath, check_format=False
    )
    for v, expected in zip(expts[0].crystal.get_unit_cell().parameters(), unit_cell):
        assert v == pytest.approx(expected)
