#include <iostream>
#include<unordered_map>
#include<set>

int duration_time;
int problem_count;
int team_count;

bool game_begin = false;
std::unordered_map<std::string, int> team_id;


struct ProblemStatus {
  //0:no AC submission yet
  //Compare needed
  int first_AC_time = 0;
  //Needed when the status is ALL
  std::string last_submission_status;

  // 0:AC 1:WA 2:RE 3:TLE 4:ALL
  int last_submission_time[5];
  //Change only before AC
  int submission_count = 0;
  //Change only when first AC
  int penality_time = 0;
  /*
   *To submit and change a problem status
   */
  inline void submit(const std::string &a, const int &b) {
    last_submission_status = a;
    if (!first_AC_time) {
      first_AC_time = b;
      penality_time = b + 20 * submission_count;
    } else {
      submission_count++;
    }
    switch(a[0]) {
      case 'A': {
        last_submission_time[0]=b;
        break;
      }
      case 'W': {
        last_submission_time[1]=b;
        break;
      }
      case 'R': {
        last_submission_time[2]=b;
        break;
      }
      case 'T': {
        last_submission_time[3]=b;
        break;
      }
      default: {
        break;
      }
    }
    last_submission_time[4]=b;
  }

};


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string str, tmp;
  int num1, num2;
  while (std::cin >> str) {
    switch (str[1]) {
      case 'D' {
      std::cin >> tmp;
      if (game_begin) {
        std::cout << "[Error]Add failed: competition has started.\n";
        break;
      }
      team_count++;
      if (!team_id.insert(std::pair<std::string, int>(tmp, team_count)).second) {
        team_count--;
        std::cout << "[Error]Add failed: duplicated team name.\n";
        break;
      }
      std::cout << "[Info]Add successfully.\n";
      break;
    }
      case 'T': {
        std::cin >> tmp >> num1 >> tmp >> num2;
        if (game_begin) {
          std::cout << "[Error]Start failed: competition has started.\n";
          break;
        }
        duration_time = num1, problem_count = num2;
        std::cout << "[Info]Competition starts.\n";
        game_begin = true;
        break;
      }
      case 'U': {
      }
    }
  }
  return 0;
}