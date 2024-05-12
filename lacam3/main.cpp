#include <iostream>
#include <lacam.hpp>

extern "C" {
    int run_lacam(const char* map_content_cstr, const char* scene_content_cstr, 
                  const char* output_name_cstr, int N);
}

int run_lacam(const char* map_content_cstr, const char* scene_content_cstr, 
              const char* output_name_cstr, int N)
{
  std::string map_content(map_content_cstr);
  std::string scene_content(scene_content_cstr);
  std::string output_name(output_name_cstr);

//   const int N = std::stoi("1");
  const int seed = 0;
  const int verbose = 0;
  const int time_limit_sec = 3;
  const bool log_short = false;

  // Solver parameters
  const bool flg_no_all = false;
  const bool flg_no_star = false;
  const bool flg_no_swap = false;
  const bool flg_no_multi_thread = false;
  const int pibt_num = 10;
  const bool flg_no_refiner = false;
  const int refiner_num = 4;
  const bool flg_no_scatter = false;
  const int scatter_margin = 10;
  const float random_insert_prob1 = 0.001f;
  const float random_insert_prob2 = 0.01f;
  const bool flg_random_insert_init_node = false;
  const float recursive_rate = 0.2f;
  const int recursive_time_limit = 1000;
  const int checkpoints_duration = 5000;

  // setup instance
//   const auto ins = scene_name.size() > 0 ? Instance(scene_content, map_content, N)
//                                         : Instance(map_content, N, seed);
  const auto ins = Instance(scene_content, map_content, N);
  if (!ins.is_valid(1)) return 1;

  // solver parameters
  Planner::FLG_SWAP = !flg_no_swap && !flg_no_all;
  Planner::FLG_STAR = !flg_no_star && !flg_no_all;
  Planner::FLG_MULTI_THREAD = !flg_no_multi_thread && !flg_no_all;
  Planner::PIBT_NUM = flg_no_all ? 1 : pibt_num;
  Planner::FLG_REFINER = !flg_no_refiner && !flg_no_all;
  Planner::REFINER_NUM = refiner_num;
  Planner::FLG_SCATTER = !flg_no_scatter && !flg_no_all;
  Planner::SCATTER_MARGIN = scatter_margin;
  Planner::RANDOM_INSERT_PROB1 = flg_no_all ? 0 : random_insert_prob1;
  Planner::RANDOM_INSERT_PROB2 = flg_no_all ? 0 : random_insert_prob2;
  Planner::FLG_RANDOM_INSERT_INIT_NODE = flg_random_insert_init_node && !flg_no_all;
  Planner::RECURSIVE_RATE = flg_no_all ? 0 : recursive_rate;
  Planner::RECURSIVE_TIME_LIMIT = flg_no_all ? 0 : recursive_time_limit;
  Planner::CHECKPOINTS_DURATION = checkpoints_duration;

  // solve
  const auto deadline = Deadline(time_limit_sec * 1000);
  const auto solution = solve(ins, verbose - 1, &deadline, seed);
  const auto comp_time_ms = deadline.elapsed_ms();

  // failure
  if (solution.empty()) info(1, verbose, &deadline, "failed to solve");

  // check feasibility
  if (!is_feasible_solution(ins, solution, verbose)) {
    info(0, verbose, &deadline, "invalid solution");
    return 1;
  }

  // post processing
  print_stats(verbose, &deadline, ins, solution, comp_time_ms);
  make_log(ins, solution, output_name, comp_time_ms, "tmp_map", seed, log_short);
  return 0;
}

int main(int argc, char *argv[]) {
    const char* map_name = "tmp.map";
    const char* scene_name = "tmp.scene";
    const char* output_name = "tmp_results.txt";
    const int N = 1;

    std::string map_content;
    std::string scene_content;

    std::ifstream map_file(map_name);
    if (map_file) {
        std::stringstream buffer;
        buffer << map_file.rdbuf();
        map_content = buffer.str();
        map_file.close();
    } else {
        std::cerr << "Failed to open map_name: " << map_name << std::endl;
        return 1;
    }

    std::ifstream scen_file(scene_name);
    if (scen_file) {
        std::stringstream buffer;
        buffer << scen_file.rdbuf();
        scene_content = buffer.str();
        scen_file.close();
    } else {
        std::cerr << "Failed to open scene_name: " << scene_name << std::endl;
        return 1;
    }
  
    const char* map_content_cstr = map_content.c_str();
    const char* scene_content_cstr = scene_content.c_str();

    return run_lacam(map_content_cstr, scene_content_cstr, output_name, N);
}

