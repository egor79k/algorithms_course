#include <chrono>
#include <cstdio>
#include <fstream>

#include <m3i/m3i.h>

const char* RESULTS_CSV_FILE = "experiment.csv";

struct ExperimentType {
    int size;
    int count;
    const char* name;
};

const int EXPERIMENTS_NUM = 3;
ExperimentType EXPERIMENT_TYPES[EXPERIMENTS_NUM] =
    {{10, 15, "small"},
    {200, 15, "medium"},
    {900, 15, "huge"}};

std::chrono::nanoseconds makeExperiment(const int size) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    M3i m3i(size, size, size);

    std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        perror("Wrong arguments number. Must be 2");
        return 1;
    }
/*
    if (*(argv[3]) < '1' || *(argv[3]) > '3') {
        perror("Wrong experiment size. Must be '1', '2' or '3'");
        return 1;
    }

    const ExperimentType& exp_type = EXPERIMENT_TYPES[*(argv[3]) - '1'];
*/
    //FILE* results_csv = fopen(RESULTS_CSV_FILE, "a");

    std::ofstream results_csv(RESULTS_CSV_FILE, std::ios::app);

    if (!results_csv.is_open()) {
        perror("Can't open file");
        return 1;
    }

    for (int exp_id = 0; exp_id < EXPERIMENTS_NUM; ++exp_id) {
        const ExperimentType& exp_type = EXPERIMENT_TYPES[exp_id];
        
        for (int i = 0; i < exp_type.count; ++i) {
            results_csv << argv[1] << ',' << argv[2] << ',' << exp_type.name << ',' << makeExperiment(exp_type.size).count() << '\n';
        }
    }

    results_csv.close();

    return 0;
}