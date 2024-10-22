#include <bits/stdc++.h>
using namespace std;

string name[15005];
int duration_time = 100000;
int problem_count = 26;
int current_time=1;

string generateRandomString(int length) {
    static const char characters[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "_";
    string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i) {
        result += characters[rand() % (sizeof(characters) - 1)];
    }
    return result;
}

int generateRandomTime() {
  current_time += rand() % 6;
  return current_time; // Random time between 1 and 100000
}

const std::vector<std::string> statuses = {"Accepted", "Wrong_Answer", "Runtime_Error", "Time_Limit_Exceed","ALL"};
const string problems[40] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
    "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z","ALL",
    "ALL","ALL","ALL","ALL","ALL","ALL","ALL","ALL","ALL","ALL","ALL","ALL","ALL"};


void generateNames() {
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < 12000; ++i) {
        int length = rand() % 20 + 1; // Generate a random length between 1 and 20
        name[i] = generateRandomString(length);
    }
}


int main() {
    srand(time(0));
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
    generateNames();
     // Seed for random number generation
    int num_commands = 300000; // Number of commands to generate

  for(int i=0;i<10000;++i){
    num_commands--;
    std::cout << "ADDTEAM " << name[i] <<"\n";
   }
  num_commands--;
  std::cout << "START DURATION " << duration_time << " PROBLEM " << problem_count << "\n";
 while(num_commands--){

        int command_type = rand() % 10000+1; // Random command type
     if (command_type >= 1 && command_type <= 500) {
         std::cout << "ADDTEAM " << name[rand() % 12000] << "\n";
     } else if (command_type > 500 && command_type <= 1000) {
         int duration_time = rand() % 10000 + 1;
         int problem_count = rand() % 26 + 1;
         std::cout << "START DURATION " << duration_time << " PROBLEM " << problem_count << "\n";
     } else if (command_type > 1000 && command_type <= 4000) {
         int time = generateRandomTime();
         std::cout << "SUBMIT " << problems[rand() % 26] << " BY " << name[rand() % 10000] << " WITH " << statuses[rand() % 4] << " AT " << time << "\n";
     } else if (command_type > 9500 && command_type <= 9700) {
         std::cout << "FLUSH" << "\n";
     } else if (command_type  > 9990 && command_type <= 10000) {
         std::cout << "FREEZE" << "\n";
     } else if (command_type > 9700 && command_type <= 9990) {
         std::cout << "SCROLL" << "\n";
     } else if (command_type > 4000 && command_type <= 6000) {
         std::cout << "QUERY_RANKING " << name[rand() % 12000] << "\n";
     } else if (command_type > 6000 && command_type <= 9500) {
         std::cout << "QUERY_SUBMISSION " << name[rand() % 12000] << " WHERE PROBLEM=" << problems[rand() % 40] << " AND STATUS=" << statuses[rand() % 5] << "\n";
     }
    }

    std::cout << "END" << "\n";

    return 0;
}