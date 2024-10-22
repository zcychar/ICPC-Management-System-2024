#include <bitset>
#include <iostream>
#include <queue>
#include<unordered_map>
#include<set>
#include <utility>
#include<vector>
#include<algorithm>

int duration_time;
int problem_count;
int team_count;

bool game_begin = false;
bool game_freeze = false;

const std::string return_value[4] = {"Accepted", "Wrong_Answer", "Runtime_Error", "Time_Limit_Exceed"};

/**
 * @note team_rank: 1-base
 * @note problem_name: 0-base
 * @note team_id: 0_base
 */

///Get the Status_ID
inline int GetId(const char ch) {
  int id;
  switch (ch) {
    case 'r': {
      id = 1;
      break;
    }
    case 'u': {
      id = 2;
      break;
    }
    case 'i': {
      id = 3;
      break;
    }
    case 'L': {
      id = 4;
      break;
    }
    case 'c': {
      id = 0;
      break;
    }
    default: {
      break;
    }
  }
  return id;
}

struct Problem {
  int AC_time = 0;
  int penalty = 0;
  int last_status = -1;
  ///0:AC 1:WA 2:RE 3:TLE 4:ALL
  int submission_time[5] = {0, 0, 0, 0, 0};
  ///Change only before AC
  int submission_count = 0;

  inline void Record(const int &status, const int &time) {
    submission_time[4] = time;
    submission_time[status] = time;
    last_status = status;
  }

  ///The function won't be called when AC_time!=0
  inline void Rank(const int &status, const int &time) {
    if (status == 0) {
      AC_time = time;
      penalty = 20 * submission_count + time;
    } else {
      submission_count++;
    }
  }
};

struct Team {
  int AC_sum = 0;
  long long penalty_sum = 0;
  ///This factor won't change unless flush or unfreeze
  int rank = 0;
  std::string name;
  std::vector<int> last_AC_time = {};
  Problem problem[35];

  ///0:AC 1:WA 2:RE 3:TLE 4:ALL
  int last_submit[5] = {-1, -1, -1, -1, -1};
  std::bitset<26> frozen_problem;

  explicit Team(std::string &s): name(s) {
  }

  struct Buffer {
    int submission_sum = 0;
    int AC_time = 0;
    int before_AC_sum = 0;

    inline void insert(const int &status, const int &time) {
      submission_sum++;
      if (AC_time) {
        return;
      }
      if (status == 0) {
        AC_time = time;
        return;
      }
      before_AC_sum++;
    }
  } buffer[35];

  inline void ProblemPrint(const int &id) {
    if (!problem[id].submission_count) {
      if (problem[id].AC_time) {
        std::cout << "+ ";
      } else if (!frozen_problem[id]) {
        std::cout << ". ";
      } else {
        std::cout << "0/" << buffer[id].submission_sum << " ";
      }
    } else {
      if (problem[id].AC_time) {
        std::cout << "+" << problem[id].submission_count << " ";
      } else if (!frozen_problem[id]) {
        std::cout << "-" << problem[id].submission_count << " ";
      } else {
        std::cout << "-" << problem[id].submission_count << "/" << buffer[id].submission_sum << " ";
      }
    }
  }

  inline void Print() {
    std::cout << name << " " << rank << " " << AC_sum << " " << penalty_sum << " ";
    for (int i = 0; i < problem_count; ++i) {
      ProblemPrint(i);
    }
    std::cout << "\n";
  }

  inline void RecordUpdate(const int &name_id, const int &submit_status, const int &time) {
    last_submit[4] = name_id;
    last_submit[submit_status] = name_id;
    problem[name_id].Record(submit_status, time);
  }

  /**
   * The function will be called only when Problem_status!=AC
   * @warning The dynamic_rank of team may change when an AC is submitted
   */
  inline void RankUpdate(const int &id, const int &status, const int &time) {
    problem[id].Rank(status, time);
    if (status == 0) {
      penalty_sum += problem[id].penalty;
      AC_sum++;
      last_AC_time.push_back(time);
    }
  }

  /**
   *The submission will be divided into two parts
   *(i)Record:this part will always be up to date(For submission check)
   *(ii)Rank:this part will be influenced by FREEZE and SCROLL
   *@param name_id = problem_name-'A'
   */
  inline void Submit(const int &name_id, const int &submit_status, const int &time) {
    RecordUpdate(name_id, submit_status, time);

    if (problem[name_id].AC_time) {
      return;
    }
    if (game_freeze) {
      frozen_problem[name_id] = true;
      buffer[name_id].insert(submit_status, time);
    } else {
      RankUpdate(name_id, submit_status, time);
    }
  }

  /**
   * (i)problem = ALL status = ALL last_submit[4].last_submit[4]
   * (ii)problem = ALL status = X last_submit[X].last_submit[4]
   * (iii)problem = X status = ALL X.last submit[4]
   * (iv)problem = X status = Y X.last submit[Y]
   */
  inline void Query(const std::string &problem_name, const int &status_id) {
    int position;
    if (problem_name == "PROBLEM=ALL") {
      position = last_submit[status_id];
    } else {
      position = (problem_name[8] - 'A');
    }
    if (position == -1 || !problem[position].submission_time[status_id]) {
      std::cout << "Cannot find any submission.\n";
      return;
    }
    std::cout << name << " " << char('A' + position) << " " << (status_id == 4
                                                                  ? return_value[problem[position].last_status]
                                                                  : return_value[status_id])
        << " " << problem[position].submission_time[status_id] << "\n";
  }

  /**
   *This function will refresh BUFFER[position] and update rank
   *@note The AC submit time WILL NOT BE IN A TIME ORDER in this function
   */
  inline void Unfroze() {
    int position = frozen_problem._Find_first();
    if (buffer[position].AC_time) {
      problem[position].AC_time = buffer[position].AC_time;
      problem[position].submission_count += buffer[position].before_AC_sum;
      problem[position].penalty = 20 * problem[position].submission_count + problem[position].AC_time;
      penalty_sum += problem[position].penalty;
      AC_sum++;
      last_AC_time.push_back(problem[position].AC_time);
      std::sort(last_AC_time.begin(),last_AC_time.end());
    } else {
      problem[position].submission_count += buffer[position].submission_sum;
    }
    buffer[position].AC_time = 0;
    buffer[position].before_AC_sum = 0;
    buffer[position].submission_sum = 0;
    frozen_problem[position] = 0;
  }
};

std::unordered_map<std::string, int> team_id;
std::vector<Team> team;
std::vector<int> current_rank;

struct CustomCompare {
  bool operator()(const int &a, const int &b) const {
    if (team[a].AC_sum != team[b].AC_sum) {
      return team[a].AC_sum > team[b].AC_sum;
    }
    if (team[a].penalty_sum != team[b].penalty_sum) {
      return team[a].penalty_sum < team[b].penalty_sum;
    }
    for (int i = team[a].AC_sum - 1; i >= 0; i--) {
      if (team[a].last_AC_time[i] != team[b].last_AC_time[i]) {
        return team[a].last_AC_time[i] < team[b].last_AC_time[i];
      }
    }
    return team[a].name < team[b].name;
  }
};

std::set<int, CustomCompare> dynamic_rank;

void ADD() {
  std::string name;
  std::cin >> name;
  if (game_begin) {
    std::cout << "[Error]Add failed: competition has started.\n";
    return;
  }
  team_count++;
  if (!team_id.insert(std::pair<std::string, int>(name, team_count - 1)).second) {
    team_count--;
    std::cout << "[Error]Add failed: duplicated team name.\n";
    return;
  }
  //Team_id equals team_count-1
  team.emplace_back(name);
  dynamic_rank.insert(team_count - 1);
  std::cout << "[Info]Add successfully.\n";
}

void FLUSH() {
  int count = 0;
  current_rank.clear();
  for (auto &it : dynamic_rank) {
    team[it].rank = ++count;
    current_rank.push_back(it);
  }
}

void START() {
  int num1, num2;
  std::string tmp;
  std::cin >> tmp >> num1 >> tmp >> num2;
  if (game_begin) {
    std::cout << "[Error]Start failed: competition has started.\n";
    return;
  }
  duration_time = num1, problem_count = num2;
  std::cout << "[Info]Competition starts.\n";
  game_begin = true;
  FLUSH();
}

/**
 * @note a betterment:The rank won't change unless an AC is submitted and game_freeze=false
 */
void SUBMIT() {
  std::string team_name, submit_status, tmp;
  char problem_name;
  int time;
  std::cin >> problem_name >> tmp >> team_name >> tmp >> submit_status >> tmp >> time;
  int id = GetId(submit_status[1]);
  auto current_id = team_id.find(team_name)->second;
  dynamic_rank.erase(current_id);
  team[current_id].Submit(int(problem_name - 'A'), id, time);
  dynamic_rank.insert(current_id);
}

void QUERY_RANKING() {
  std::string name;
  std::cin >> name;
  auto it = team_id.find(name);
  if (it == team_id.end()) {
    std::cout << "[Error]Query ranking failed: cannot find the team.\n";
  } else {
    std::cout << "[Info]Complete query ranking.\n";
    if (game_freeze) {
      std::cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.\n";
    }
    std::cout << name << " NOW AT RANKING " << team[it->second].rank << "\n";
  }
}

void QUERY_SUBMISSION() {
  std::string team_name, problem_name, status, tmp;
  std::cin >> team_name >> tmp >> problem_name >> tmp >> status;
  auto it = team_id.find(team_name);
  if (it == team_id.end()) {
    std::cout << "[Error]Query submission failed: cannot find the team.\n";
  } else {
    std::cout << "[Info]Complete query submission.\n";
    team[it->second].Query(problem_name, GetId(status[8]));
  }
}

void FREEZE() {
  if (game_freeze) {
    std::cout << "[Error]Freeze failed: scoreboard has been frozen.\n";
    return;
  }
  game_freeze = true;
  std::cout << "[Info]Freeze scoreboard.\n";
}

void SCROLL() {
  if (!game_freeze) {
    std::cout << "[Error]Scroll failed: scoreboard has not been frozen.\n";
    return;
  }
  game_freeze = false;
  std::cout << "[Info]Scroll scoreboard.\n";
  FLUSH();
  for (auto id : current_rank) {
    team[id].Print();
    // std::cout << team[id].frozen_problem.count() << std::endl;
  }
  auto it = --dynamic_rank.end();
  /**
   * main function to operate scroll
   * @param it :the iterator points to operating element
   * @param next_it :record position and judge whether unfroze causes rank change
   * @param prev_it :record for rank change
   */
  while (1) {
    while (!team[*it].frozen_problem.any() && it != dynamic_rank.begin()) {
      --it;
    }
    if (it == dynamic_rank.begin()) {
      break;
    }
    auto next_it = std::next(it), prev_it = std::prev(it);
    int current_id = *it;
    dynamic_rank.erase(it);
    team[current_id].Unfroze();
    auto refresh_it = dynamic_rank.insert(current_id).first;
    if (std::next(refresh_it) != next_it) {
      std::cout << team[current_id].name << " " << team[*std::next(refresh_it)].name << " " << team[current_id].AC_sum
          << " " << team[current_id].penalty_sum << "\n";
      it = prev_it;
    } else {
      it = refresh_it;
    }
  }
  while (team[*it].frozen_problem.any()) {
    team[*it].Unfroze();
  }
  FLUSH();
  for (auto id : current_rank) {
    team[id].Print();
  }
}


int main() {
  // freopen("1.in", "r", stdin);
  // freopen("out.txt", "w", stdout);
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string str;
  while (std::cin >> str) {

    switch (str[1]) {
      case 'D': {
        ADD();
        break;
      }
      case 'T': {
        START();
        break;
      }
      case 'N': {
        std::cout << "[Info]Competition ends.\n";
        return 0;
      }
      case 'L': {
        std::cout << "[Info]Flush scoreboard.\n";
        FLUSH();
        break;
      }
      case 'R': {
        FREEZE();
        break;
      }
      case 'C': {
        SCROLL();
        break;
      }
      default: {
        if (str == "QUERY_RANKING") {
          QUERY_RANKING();
        } else if (str == "QUERY_SUBMISSION") {
          QUERY_SUBMISSION();
        } else {
          SUBMIT();
        }
        break;
      }
    }
  }
  return 0;
}