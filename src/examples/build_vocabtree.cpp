

/** \file build_vocabtree.cpp
 * 	\brief build the vocabtree for querying
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include "gflags/gflags.h"

#include "libvot_config.h"
#include "vocab_tree/vot_pipeline.h"
#include "utils/io_utils.h"

//TODO(tianwei): figure out a way to do platform-dependent MAX_ARRAY_SIZE
//#define MAX_ARRAY_SIZE 8388608  // 2^23
using namespace std;

/* 
sift_type: 0 - our own sift data format
		   1 - vlfeat sift (in openMVG format)
		   2 - TODO(tianwei): lowe's sift type
*/

int main(int argc, char **argv)
{
	fprintf(stdout, "libvot version: %d.%d.%d\n", LIBVOT_VERSION_MAJOR, LIBVOT_VERSION_MINOR, LIBVOT_VERSION_PATCH);
	google::ParseCommandLineFlags(&argc, &argv, true);
	if (argc < 3) {
		printf("Usage: %s <sift_list> <output_dir> [depth] [branch_num] [sift_type] [thread_num]\n", argv[0]);
		return -1;
	}

	const char *sift_input_file = argv[1];
	const char *output_dir = argv[2];
	std::string output_directory = std::string(output_dir);
	std::string tree_output = tw::IO::JoinPath(output_directory, std::string("tree.out"));
	std::string db_output = tw::IO::JoinPath(output_directory, std::string("db.out"));
	// std::string match_output = tw::IO::JoinPath(output_directory, std::string("match.out"));
	// std::string filtered_output = tw::IO::JoinPath(output_directory, std::string("match_pairs"));
	const std::string svg_adjacency_matrix = tw::IO::JoinPath(output_directory, std::string("adjacency_matrix.svg"));

	// create folder
	tw::IO::Mkdir(output_directory);

	// optional parameters
	int depth = 6;
	int branch_num = 8;
	vot::SiftType sift_type = vot::E3D_SIFT;
	int thread_num = std::thread::hardware_concurrency();
	int start_id = 0;

	if (argc > 3)
		depth = atoi(argv[3]);
	if (argc > 4)
		branch_num = atoi(argv[4]);
	if (argc > 5)
		sift_type = vot::SiftType(atoi(argv[5]));
	/*if (argc > 6)
		num_matches = atoi(argv[6]);*/
	if (argc > 6)
		thread_num = atoi(argv[6]);

	if (!vot::BuildVocabTree(sift_input_file, tree_output.c_str(), depth, branch_num, sift_type, thread_num))
		return -1;
	if (!vot::BuildImageDatabase(sift_input_file, tree_output.c_str(), db_output.c_str(), sift_type, start_id, thread_num))
		return -1;
/*
    if (!vot::QueryDatabase(db_output.c_str(), sift_input_file, match_output.c_str(), sift_type, thread_num))
		return -1;
	if(!vot::FilterMatchList(sift_input_file, match_output.c_str(), filtered_output.c_str(), num_matches, svg_adjacency_matrix.c_str()))
		return -1;
*/
	return 0;
}
