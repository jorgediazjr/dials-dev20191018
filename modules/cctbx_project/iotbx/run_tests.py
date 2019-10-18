from __future__ import absolute_import, division, print_function
from libtbx import test_utils
import sys
import libtbx.load_env

tst_list_base = [
  "$D/regression/tst_wildcard.py",
  "$D/regression/tst_simple_parser.py",
  "$D/regression/tst_phil.py",
  "$D/regression/tst_pdb_cif_inputs.py",
  "$D/regression/tst_data_manager.py",
  "$D/ranges.py",
  "$D/regression/tst_crystal_symmetry_from_any.py",
  "$D/regression/tst_poscar.py",
  "$D/kriber/tst_strudat.py",
  "$D/cif/tests/tst_geometry.py",
  "$D/cif/tests/tst_lex_parse_build.py",
  "$D/cif/tests/tst_model.py",
  "$D/cif/tests/tst_restraints.py",
  "$D/cif/tests/tst_validation.py",
  "$D/cif/tests/tst_ucif_examples_compilation.py",
  "$D/cif/tests/tst_parser.py",
  "$D/cif/tests/tst_citations.py",
  "$D/shelx/tst_lex_parse_build.py",
  "$D/shelx/tst_hklf.py",
  "$D/shelx/tst_writer.py",
  "$D/shelx/tst_fvar_encoding.py",
  "$D/pdb/tst_mmcif.py",
  "$D/pdb/tst_mmcif_hierarchy.py",
  "$D/pdb/tst_tls.py",
  ["$D/pdb/hybrid_36.py", "exercise"],
  "$B/pdb/hybrid_36_fem",
  "$D/pdb/tst_hierarchy.py",
  "$D/pdb/tst_hierarchy_atom_sort.py",
  "$D/pdb/tst_ext.py",
  "$D/pdb/tst_atom_selection.py",
  "$D/pdb/tst_rna_dna_atom_names.py",
  "$D/pdb/tst_atom_name_interpretation.py",
  "$D/pdb/tst_extract_rfactors_resolutions_sigma.py",
  "$D/pdb/modified_aa_names.py",
  "$D/pdb/modified_rna_dna_names.py",
  "$D/regression/secondary_structure/tst_sheet.py",
  "$D/regression/secondary_structure/tst_annotation.py",
  "$D/pdb/secondary_structure.py",
  "$D/pdb/tst_atom_selection_string.py",
  "$D/pdb/tst_secondary_structure.py",
  "$D/pdb/tst_utils.py",
  "$D/pdb/tst_secondary_structure_2.py",
  "$D/examples/pdb_to_map_simple.py",
  "$D/examples/pdb_truncate_to_ala/tst.py",
  "$D/examples/pdb_tardy_conf_sampling_simple.py",
  "$D/cns/space_group_symbols.py",
  "$D/cns/tst_cns.py",
  ["$D/scalepack/tst_merge.py", "P31"],
  "$D/scalepack/no_merge_original_index.py",
  "$D/ccp4_map/tst.py",
  "$D/mtz/tst_ext.py",
  "$D/mtz/tst_extract_from_symmetry_lib.py",
  "$D/mtz/tst_dano.py",
  "$D/mtz/tst_miller_dict.py",
  "$D/mtz/tst_unmerged.py",
  ["$D/mtz/tst.py", "P31"],
  "$D/examples/tst_mtz_free_flipper.py",
  "$D/regression/tst_reflection_file_utils.py",
  "$D/detectors/tst_adsc.py",
  "$D/detectors/tst_detectors.py",
  "$D/xplor/tst_xplormap.py",
  ["$D/regression/tst_phases.py", "P31"],
  "$D/regression/tst_pdbx_mmcif_tutorial.py",
  "$D/regression/tst_lattice_symmetry.py",
  ["$D/regression/tst_reflection_statistics.py", "Fdd2 P31m"],
  "$D/regression/tst_data_plots.py",
  "$D/regression/tst_csv_utils.py",
  "$D/regression/tst_file_reader.py",
  "$D/regression/tst_bioinformatics.py",
  "$D/regression/tst_box_around_molecule.py",
  "$D/regression/tst_mtz_as_cif.py",
  "$D/regression/tst_mmcif_segids.py",
  "$D/regression/tst_add_conformations.py",
  "$D/regression/tst_symmetry.py",
  "$D/regression/tst_reindex.py",
  "$D/regression/tst_reflection_file_editor.py",
  "$D/regression/tst_split_models.py",
  "$D/regression/tst_pdb_as_fasta.py",
  "$D/regression/tst_merging_statistics.py",
  "$D/regression/tst_simple_map_coefficients.py",
  "$D/regression/tst_sort_atoms.py",
  "$D/xds/tests/tst_xparm.py",
  "$D/xds/tests/tst_xds_inp.py",
  "$D/xds/tests/tst_integrate_hkl.py",
  "$D/xds/tests/tst_spots_xds.py",
  "$D/regression/tst_pdb_as_cif.py",
  "$D/scalepack/tst_no_merge_original_index.py",
  "$D/regression/tst_export_scalepack_unmerged.py",
  ["$D/dsn6/tst.py", "P31"],
  "$D/regression/ncs/tst_mtrix_biomt_cmdl.py",
  "$D/regression/ncs/tst_ncs_search_ligs.py",
  "$D/regression/ncs/tst_ncs_search_broken_chain.py",
  "$D/regression/ncs/tst_ncs_groups_preprocessing.py",
  "$D/regression/ncs/tst_ncs_input.py",
  "$D/regression/ncs/tst_ncs_user_selections.py",
  "$D/regression/ncs/tst_ncs.py",
  "$D/pdb/tst_read_mtrix_records_from_cif.py",
  "$D/regression/tst_show_systematic_absences.py",
  "$D/regression/tst_miller_sort_asu.py",
  "$D/regression/tst_reflection_file_reader.py",
  "$D/regression/tst_xray_scale.py",
  "$D/bioinformatics/test/tst_alignment_as_hsearch.py",
  "$D/bioinformatics/test/tst_ebi_wu_blast_xml.py",
  "$D/bioinformatics/test/tst_ncbi_blast_xml.py",
  "$D/regression/tst_cif_as_pdb_1atom.py",
  ]

# failing tests on Windows, Python 2.7
tst_list_windows_fail = [
  "$D/detectors/tst_debug_write.py",
]

tst_list_fail = list()
if sys.platform == 'win32':
  tst_list_fail += tst_list_windows_fail
else:
  tst_list_base += tst_list_windows_fail

# failing tests on macOS and linux, Python 3.6
tst_list_unix_fail = [
  "$D/pdb/tst_pdb.py",
  ]

if ((sys.platform == 'darwin' or sys.platform.startswith('linux')) and
    sys.version_info > (3, 0)):
  tst_list_fail += tst_list_unix_fail
else:
  tst_list_base += tst_list_unix_fail

# final lists
tst_list = tst_list_base
tst_list_expected_failures = tst_list_fail

def run():
  build_dir = libtbx.env.under_build("iotbx")
  dist_dir = libtbx.env.dist_path("iotbx")

  test_utils.run_tests(build_dir, dist_dir, tst_list)

if (__name__ == "__main__"):
  run()