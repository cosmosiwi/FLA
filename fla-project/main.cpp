#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

using namespace std;

int verbose = 0;

class PDA {
public:
  PDA() {
    Q = set<string>();
    sigma = set<string>();
    sigma.insert("_");
    gamma = set<string>();
    gamma.insert("_");
    initialState = "";
    initialStack = "";
    finalState = set<string>();
    illegalInput[" "] = 1;
    illegalInput[","] = 1;
    illegalInput[";"] = 1;
    illegalInput["{"] = 1;
    illegalInput["}"] = 1;
    illegalInput["*"] = 1;
    illegalInput["_"] = 1;
  }
  ~PDA() {
    Q.clear();
    sigma.clear();
    gamma.clear();
    finalState.clear();
    delta.clear();
  };
  void setInput(string input) { this->input = input; };
  void addStateSet(string state) { Q.insert(state); };
  void addInputSet(string input) {
    if (!checkAddInput(input)) {
      handleSyntaxError("input is illegal");
      return;
    }
    sigma.insert(input);
  };
  void addStackSet(string stack) {
    if (!checkAddStatck(stack)) {
      handleSyntaxError("stack is illegal");
      return;
    }
    gamma.insert(stack);
  };
  void addInitialState(string state) { initialState = state; };
  void addInitialStack(string stack) { initialStack = stack; };
  void addFinalStateSet(string state) { finalState.insert(state); };
  void addTransition(string state, string input, string stack, string nextState,
                     string nextStack) {
    delta[make_tuple(state, input, stack)] = make_pair(nextState, nextStack);
  };
  int run(); // -1 illegal input, 0 not accepted, 1 accepted
  void handleSyntaxError(string message) {
    std::cerr << "syntax error : " << message << std::endl;
    exit(1);
  };
  bool checkIllegalInput(string input) {
    for (auto s : input) {
      if (illegalInput.find(std::string(1, s)) != illegalInput.end()) {
        return false;
      }
    }
    return true;
  }
  bool checkAddInput(string input) { return checkIllegalInput(input); }
  bool checkAddStatck(string stack) { return checkIllegalInput(stack); }
  bool checkInput(string input) {
    for (auto s : input) {
      if (sigma.find(std::string(1, s)) == sigma.end()) {
        return false;
      }
    }
    return true;
  };
  bool checkStack(string stack) {
    for (auto s : stack) {
      if (gamma.find(std::string(1, s)) == gamma.end()) {
        return false;
      }
    }
    return true;
  };
  bool checkState(string state) {
    if (Q.find(state) == Q.end()) {
      return false;
    }
    return true;
  };
  void printLog(string currentState, string currentStack, int step) {
    cout << "Input\t: " << this->input << endl;
    cout << "Current\t: ";
    for (int i = 0; i < input.size(); i++) {
      if (i == step) {
        cout << "^";
      } else {
        cout << " ";
      }
    }
    cout << endl;
    cout << "State\t: " << currentState << endl;
    cout << "Stack\t: " << currentStack << endl;
    cout << "---------------------------------" << endl;
  }

private:
  set<string> Q;     // set of states
  set<string> sigma; // input alphabet
  set<string> gamma; // stack alphabet
  string initialState;
  string initialStack;
  set<string> finalState;
  struct TupleHasher {
    template <typename T1, typename T2, typename T3>
    std::size_t operator()(const std::tuple<T1, T2, T3> &t) const {
      auto h1 = std::hash<T1>{}(std::get<0>(t));
      auto h2 = std::hash<T2>{}(std::get<1>(t));
      auto h3 = std::hash<T3>{}(std::get<2>(t));
      return h1 ^ (h2 << 1) ^ (h3 << 2); // 组合哈希值
    }
  };
  unordered_map<tuple<string, string, string>, pair<string, string>,
                TupleHasher>
      delta; // transition function Q x sigma x gamma -> Q x gamma*
  string input;

  unordered_map<string, int> illegalInput;
};

int PDA::run() {
  string currentState = initialState;
  string currentStack = initialStack;
  for (auto s : this->input) {
    string input = std::string(1, s);
    if (illegalInput[input] || this->sigma.find(input) == this->sigma.end()) {
      if(verbose){
        cout << "illegal input : " << input << endl;
        cout << "Input\t: " << this->input << endl;
        cout << "Index\t: ";
        for (auto c : this->input) {
          if (c == s) {
            cout << "^";
          } else {
            cout << " ";
          }
        }
        cout << endl;
        cout << "------------------------" << endl;
      }
      return -1;
    }
  }
  this->input += "_";
  int step = 0;
  for (auto s : this->input) {
    string input = std::string(1, s);
    string top = currentStack.substr(0, 1);
    if (verbose) {
      printLog(currentState, currentStack, step);
    }
    pair<string, string> nextState =
        delta[make_tuple(currentState, input, top)];
    currentStack = nextState.second == "_"
                       ? currentStack.substr(1)
                       : nextState.second + currentStack.substr(1);
    currentState = nextState.first;
    if (nextState == make_pair("", "")) {
      return 0;
    }
    if (finalState.find(currentState) != finalState.end()) {
      return 1;
    }
    step++;
  }
}

class TM {
public:
  TM() {
    Q = set<string>();
    sigma = set<string>();
    gamma = set<string>();
    gamma.insert("_");
    initialState = "";
    blanket = "";
    finalState = set<string>();
    input = "";
    illegalInput[" "] = 1;
    illegalInput[","] = 1;
    illegalInput[";"] = 1;
    illegalInput["{"] = 1;
    illegalInput["}"] = 1;
    illegalInput["*"] = 1;
    illegalInput["_"] = 1;
  }
  ~TM() {
    Q.clear();
    sigma.clear();
    gamma.clear();
    finalState.clear();
    delta.clear();
  };
  void setInput(string input) { this->input = input; };
  void setN(int n) {
    this->n = n;
    tape = vector<string>(n, "");
    pos = vector<int>(n, 0);
  };
  int getN() { return n; };
  void addStateSet(string state) { Q.insert(state); };
  void addInputSet(string input) {
    if (!checkAddInput(input)) {
      handleSyntaxError("input is illegal");
      return;
    }
    sigma.insert(input);
  };
  void addStackSet(string stack) {
    if (!checkAddStack(stack)) {
      handleSyntaxError("stack is illegal");
      return;
    }
    gamma.insert(stack);
  };
  void addInitialState(string state) { initialState = state; };
  void addBlanket(string stack) { blanket = stack; };
  void addFinalStateSet(string state) { finalState.insert(state); };
  void addTransition(string state, string input, string output,
                     string direction, string nextState) {
    if (!checkAddStackInTransition(output) ||
        !checkAddStackInTransition(input)) {
      handleSyntaxError(output + " " + input);
      return;
    }
    if (input.size() != n || output.size() != n) {
      handleSyntaxError("input and output size not match");
      return;
    }
    if (Q.find(state) == Q.end()) {
      handleSyntaxError(state);
      return;
    }
    if (Q.find(nextState) == Q.end()) {
      handleSyntaxError(nextState);
      return;
    }
    delta[make_pair(state, input)] = make_tuple(output, direction, nextState);
  };
  bool checkIllegalInput(string input) {
    // cout << input << " " << illegalInput["_"] << endl;
    for (auto s : input) {
      if (illegalInput[std::string(1, s)] == 1) {
        // cout << std::string(1, s) << endl;
        return false;
      }
    }
    return true;
  }
  bool checkAddInput(string input) { return checkIllegalInput(input); }
  bool checkAddStack(string stack) {
    illegalInput["_"] = 0;
    bool result = checkIllegalInput(stack);
    illegalInput["_"] = 1;
    return result;
  }
  bool checkAddStackInTransition(string stack) {
    illegalInput["_"] = 0;
    illegalInput["*"] = 0;
    bool result = true;
    for (auto s : stack) {
      string input = std::string(1, s);
      // cout << input << (gamma.find(input) == gamma.end()) << endl;
      if (!checkIllegalInput(input) || (gamma.find(input) == gamma.end() &&
                                        !(input == "_" || input == "*"))) {
        result = false;
      }
    }
    illegalInput["_"] = 1;
    illegalInput["*"] = 1;
    return result;
  }
  void run();
  void handleSyntaxError(string message) {
    std::cerr << "syntax error " << message << std::endl;
    exit(1);
  };
  void printStepLog(string state, int step) {
    cout << "State\t: " << state << endl;
    cout << "Step\t: " << step << endl;
    auto fn = [](int n) {
      if (n < 10)
        return;
      int res = 0;
      int k = n;
      while (k) {
        res++;
        k /= 10;
      }
      for (int i = 0; i < res - 1; i++)
        cout << " ";
    };
    for (int i = 0; i < this->n; i++) {
      cout << "Index" << i << "\t: ";
      for (int j = offset; j > 0; j--)
        cout << j << " ";
      for (int j = 0; j < tape[i].size() - offset; j++) {
        cout << j << " ";
      }
      cout << endl;
      cout << "Tape" << i << "\t: ";
      for (int j = 0; j < tape[i].size(); j++) {
        cout << tape[i][j] << " ";
        fn(j - offset);
      }
      cout << endl;
      cout << "Head" << i << "\t: ";
      for (int j = 0; j < tape[i].size(); j++) {
        if (j == pos[i]) {
          cout << "^ ";
        } else {
          cout << "  ";
        }
        fn(j - offset);
      }
      cout << endl;
    }
    cout << "---------------------------------" << endl;
  }
  void printErrLog(string input) {
    if (!verbose) {
      cout << "illegal input" << endl;
    } else {
      cout << "Input: " << this->input << endl;
      cout << "===============ERR===============" << endl;
      cout << "error: '" << input
           << "' was not declared in the set of input symbols" << endl;
      cout << "Input: " << this->input << endl;
      cout << "       ";
      for (int i = 0; i < this->input.size(); i++) {
        if (input[0] == this->input[i]) {
          cout << "^";
          break;
        } else
          cout << " ";
      }
      cout << endl;
      cout << "===============END===============" << endl;
    }
    exit(1);
  }

private:
  set<string> Q;          // set of states
  set<string> sigma;      // input alphabet
  set<string> gamma;      // stack alphabet
  string initialState;    // q0
  string blanket;         // B
  set<string> finalState; // F
  int n;                  // tape number;
  struct PairHasher {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2> &t) const {
      auto h1 = std::hash<T1>{}(t.first);
      auto h2 = std::hash<T2>{}(t.second);
      return h1 ^ (h2 << 1);
    }
  };
  unordered_map<pair<string, string>, tuple<string, string, string>, PairHasher>
      delta; // transition function Q x gamma -> gamma x {L, R} x State
  vector<string> tape;
  vector<int> pos;

  string input;

  int offset = 0;

  unordered_map<string, int> illegalInput;
};

void TM::run() {
  tape[0] = this->input;
  for (auto s : this->input) {
    string input = std::string(1, s);
    if (illegalInput[input] || this->sigma.find(input) == this->sigma.end()) {
      if (verbose)
        printErrLog(input);
      else {
        cout << "illegal input" << endl;
        exit(1);
      }
    }
  }
  if (verbose) {
    cout << "Input: " << this->input << endl;
    cout << "===============RUN===============" << endl;
  }
  for (int i = 1; i < this->n; i++) {
    tape[i] = this->blanket;
  }
  string currentState = initialState;
  int time = 0;
  while (true) {
    string currentInput = "";
    for (int i = 0; i < this->n; i++) {
      currentInput += "*";
    }
    tuple<string, string, string> next;
    // ****** -> a***** -> ab**** -> abc*** -> abcd** -> abcde* -> abcdef

    // What we want to do next is to build a valid input that can be used to
    // find the next state Use dfs to find the next state
    // TODO: optimize the dfs function
    std::function<bool(string &, int)> dfs = [&](string &currentInput,
                                                 int ptx) {
      if (ptx >= this->n) {
        // cout << currentInput << " asdf " << endl;
        if (delta[make_pair(currentState, currentInput)] ==
            make_tuple("", "", "")) {
          return false;
        }
        return true;
      }
      currentInput[ptx] = tape[ptx][pos[ptx]];
      if (dfs(currentInput, ptx + 1)) {
        return true;
      }
      currentInput[ptx] = '*';
      if (dfs(currentInput, ptx + 1)) {
        return true;
      }
      return false;
    };
    if (verbose == 1)
      printStepLog(currentState, time);
    int ptx = 0;
    if (finalState.find(currentState) != finalState.end() ||
        !dfs(currentInput, ptx)) {
      string result = tape[0];
      result = result.substr(result.find_first_not_of("_"));
      result = result.substr(0, result.find_last_not_of("_") + 1);
      for (auto s : result) {
        // cout << s << " ++ " << endl;
        if (s == '_')
          cout << ' ';
        else
          cout << s;
      }
      cout << endl;
      if (verbose)
        cout << "===============END===============" << endl;
      break;
    }
    next = delta[make_pair(currentState, currentInput)];
    string output = get<0>(next);
    string direction = get<1>(next);
    string nextState = get<2>(next);

    // change tape
    for (int i = 0; i < this->n; i++) {
      if (output[i] == '*')
        continue;
      tape[i][pos[i]] = output[i];
    }

    // move head of the tape
    for (int i = 0; i < this->n; i++) {
      if (direction[i] == 'l') {
        pos[i]--;
        if (pos[i] < 0) {
          offset++;
          pos[i] = 0;
          for (int j = 0; j < this->n; j++) {
            if (j != i)
              pos[j]++;
            tape[j] = "_" + tape[j];
          }
        }
      } else if (direction[i] == 'r') {
        pos[i]++;
        if (pos[i] >= tape[i].size()) {
          tape[i] += "_";
        }
      } else if (direction[i] == '*') {
        continue;
      }
    }
    currentState = nextState;
    time++;
  }
}

bool checkEqual(string line) {
  if (line != " = ") {
    return false;
  }
  return true;
}

bool checkBrace(string line) {
  if (line[0] != '{' || line[line.size() - 1] != '}') {
    return false;
  }
  return true;
}

#define HANDLE_SET_FUNCTION(name)                                              \
  bool handle##name(string line, PDA &pda, int &pos) {                         \
    if (!checkEqual(line.substr(pos + 1, 3))) {                                \
      pda.handleSyntaxError("' = ' expected");                                 \
    }                                                                          \
    string g =                                                                 \
        line.substr(line.find_first_of("{"),                                   \
                    line.find_last_of("}") - line.find_first_of("{") + 1);     \
    if (!checkBrace(g)) {                                                      \
      pda.handleSyntaxError("{...} expected");                                 \
    }                                                                          \
    string curr;                                                               \
    int lastComma = 0;                                                         \
    for (int i = 1; i < g.size() - 1; i++) {                                   \
      if (g[i] == ',') {                                                       \
        curr = g.substr(lastComma + 1, i - lastComma - 1);                     \
        pda.add##name(curr);                                                   \
        lastComma = i;                                                         \
      }                                                                        \
    }                                                                          \
    curr = g.substr(lastComma + 1, g.size() - 1 - lastComma - 1);              \
    pda.add##name(curr);                                                       \
    pos += 3 + g.size();                                                       \
    return true;                                                               \
  }

HANDLE_SET_FUNCTION(StateSet)
HANDLE_SET_FUNCTION(StackSet)
HANDLE_SET_FUNCTION(InputSet)
HANDLE_SET_FUNCTION(FinalStateSet)

#undef HANDLE_SET_FUNCTION

#define HANDLE_FUNCTION(name)                                                  \
  bool handleInitial##name(string line, PDA &pda, int &pos) {                  \
    if (line[pos + 1] == '0')                                                  \
      pos++;                                                                   \
    else {                                                                     \
      pda.handleSyntaxError("init error");                                     \
    }                                                                          \
    if (!checkEqual(line.substr(pos + 1, 3))) {                                \
      pda.handleSyntaxError("' = ' expected");                                 \
    }                                                                          \
    string q = line.substr(line.find_first_of("=") + 2);                       \
    pda.addInitial##name(q);                                                   \
    pos += 3 + q.size();                                                       \
    return true;                                                               \
  }
HANDLE_FUNCTION(State)
HANDLE_FUNCTION(Stack)
#undef HANDLE_FUNCTION

bool handleTransition(string state, string input, string stack,
                      string nextState, string nextStack, PDA &pda) {
  pda.addTransition(state, input, stack, nextState, nextStack);
  return true;
}

bool handleTransition(string state, string input, string output,
                      string direction, string nextState, TM &tm) {
  tm.addTransition(state, input, output, direction, nextState);
  return true;
}

PDA parseToPDA(string filename, string input) {
  PDA pda = PDA();
  pda.setInput(input);

  ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(1);
  }

  string line;
  while (getline(file, line)) {
    if (line[line.find_first_of(";")] == ';') {
      line = line.substr(0, line.find_first_of(";"));
    }
    while (line[line.size() - 1] == ' ') {
      line = line.substr(0, line.size() - 1);
    }
    // cout << line << endl;
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == ' ')
        continue;
      if (line[i] == '#') {
        i++;
        switch (line[i]) {
        case 'Q': {
          handleStateSet(line, pda, i);
          break;
        }
        case 'S': {
          handleInputSet(line, pda, i);
          break;
        }
        case 'G': {
          handleStackSet(line, pda, i);
          break;
        }
        case 'q': {
          handleInitialState(line, pda, i);
          break;
        }
        case 'z': {
          handleInitialStack(line, pda, i);
          break;
        }
        case 'F': {
          handleFinalStateSet(line, pda, i);
          break;
        }
        }
      } else {
        int keyCount = 0;
        int last_space = -1;
        string args[5];
        int argIndex = 0;
        for (int j = 0; j < line.size(); j++) {
          int oldJ = j;
          if (line[j] == ' ') {
            while(line[j] == ' ') {
              j++;
            }
            j--;
            keyCount++;
            args[argIndex] = line.substr(last_space + 1, oldJ - last_space - 1);
            last_space = j;
            argIndex++;
          }
        }
        args[argIndex] =
            line.substr(last_space + 1, line.size() - last_space - 1);
        if (keyCount != 4) {
          pda.handleSyntaxError("5 arguments expected");
        }
        if (!pda.checkState(args[0]) || !pda.checkInput(args[1]) ||
            !pda.checkStack(args[2]) || !pda.checkState(args[3]) ||
            !pda.checkStack(args[4])) {
          pda.handleSyntaxError("transision function error");
        }
        handleTransition(args[0], args[1], args[2], args[3], args[4], pda);
        break;
      }
    }
  }
  return pda;
}

#define TM_HANDLE_SET_FUNCTION(name)                                           \
  bool handle##name(string line, TM &tm, int &pos) {                           \
    if (!checkEqual(line.substr(pos + 1, 3))) {                                \
      tm.handleSyntaxError("' = ' expected");                                  \
    }                                                                          \
    string g =                                                                 \
        line.substr(line.find_first_of("{"),                                   \
                    line.find_last_of("}") - line.find_first_of("{") + 1);     \
    if (!checkBrace(g)) {                                                      \
      tm.handleSyntaxError("{...} expected");                                  \
    }                                                                          \
    string curr;                                                               \
    int lastComma = 0;                                                         \
    for (int i = 1; i < g.size() - 1; i++) {                                   \
      if (g[i] == ',') {                                                       \
        curr = g.substr(lastComma + 1, i - lastComma - 1);                     \
        tm.add##name(curr);                                                    \
        /*cout << curr << endl;*/                                              \
        lastComma = i;                                                         \
      }                                                                        \
    }                                                                          \
    curr = g.substr(lastComma + 1, g.size() - 1 - lastComma - 1);              \
    tm.add##name(curr);                                                        \
    /*cout << curr << endl;*/                                                  \
    pos += 3 + g.size();                                                       \
    return true;                                                               \
  }

TM_HANDLE_SET_FUNCTION(StateSet)
TM_HANDLE_SET_FUNCTION(StackSet)
TM_HANDLE_SET_FUNCTION(InputSet)
TM_HANDLE_SET_FUNCTION(FinalStateSet)

#undef TM_HANDLE_SET_FUNCTION

#define TM_HANDLE_FUNCTION(name)                                               \
  bool handle##name(string line, TM &tm, int &pos) {                           \
    if (line[pos + 1] == '0')                                                  \
      pos++;                                                                   \
    if (!checkEqual(line.substr(pos + 1, 3))) {                                \
      tm.handleSyntaxError("' = ' expected");                                  \
    }                                                                          \
    string q = line.substr(line.find_first_of("=") + 2);                       \
    tm.add##name(q);                                                           \
    /*cout << q << endl;*/                                                     \
    pos += 3 + q.size();                                                       \
    return true;                                                               \
  }
TM_HANDLE_FUNCTION(InitialState)
TM_HANDLE_FUNCTION(Blanket)
#undef HANDLE_FUNCTION

TM parseToTM(string filename, string input) {
  TM tm = TM();
  tm.setInput(input);

  ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(1);
  }

  string line;
  while (getline(file, line)) {
    if (line[line.find_first_of(";")] == ';') {
      line = line.substr(0, line.find_first_of(";"));
    }
    while (line[line.size() - 1] == ' ') {
      line = line.substr(0, line.size() - 1);
    }
    // cout << line << endl;
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == ' ')
        continue;
      if (line[i] == '#') {
        i++;
        // cout << line << endl;
        switch (line[i]) {
        case 'Q': {
          handleStateSet(line, tm, i);
          break;
        }
        case 'S': {
          handleInputSet(line, tm, i);
          break;
        }
        case 'G': {
          handleStackSet(line, tm, i);
          break;
        }
        case 'q': {
          handleInitialState(line, tm, i);
          break;
        }
        case 'B': {
          handleBlanket(line, tm, i);
          break;
        }
        case 'F': {
          handleFinalStateSet(line, tm, i);
          break;
        }
        case 'N': {
          string n = line.substr(line.find_first_of("=") + 2);
          tm.setN(stoi(n));
          // cout << tm.getN() << endl;
          i = line.size() - 1;
          break;
        }
        }
      } else {
        int keyCount = 0;
        int last_space = -1;
        string args[5];
        int argIndex = 0;
        for (int j = 0; j < line.size(); j++) {
          int oldJ = j;
          if (line[j] == ' ') {
            while(line[j] == ' ') {
              j++;
            }
            j--;
            keyCount++;
            args[argIndex] = line.substr(last_space + 1, oldJ - last_space - 1);
            last_space = j;
            argIndex++;
          }
        }
        args[argIndex] =
            line.substr(last_space + 1, line.size() - last_space - 1);
        if (keyCount != 4) {
          cout << line << endl;
          tm.handleSyntaxError("5 arguments expected");
        }
        handleTransition(args[0], args[1], args[2], args[3], args[4], tm);
        break;
      }
    }
  }
  return tm;
}

void parseAutomata(string filename, string input) {
  string type = filename.substr(filename.find_last_of(".") + 1);
  if (type == "pda") {
    PDA pda = parseToPDA(filename, input);
    int code = pda.run();
    if (code == 0) {
      cout << "false" << endl;
    } else if (code == 1) {
      cout << "true" << endl;
    } else {
      cout << "illegal input" << endl;
    }
  } else if (type == "tm") {
    TM tm = parseToTM(filename, input);
    tm.run();
    // tm.run();
  } else {
    std::cerr << "Error: unknown file type" << std::endl;
    exit(1);
  }
}

void printHelpInfo() {
  std::cout << "usage:\tfla [-h|--help] <pda> <input>\n\tfla [-v|--verbose] "
               "[-h|--help] <tm> <input>\n";
}

void handleError() {
  printHelpInfo();
  exit(1);
}

void parseArguments(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    string arg(argv[i]);
    if (arg == "-h" || arg == "--help") {
      printHelpInfo();
      exit(0);
    } else if (arg == "-v" || arg == "--verbose") {
      if (verbose == 1)
        handleError();
      verbose = 1;
      if (i == argc - 1) {
        handleError();
      }
    } else {
      if (i != argc - 2) {
        handleError();
      }
      parseAutomata(arg, argv[i + 1]);
      break;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc == 1) {
    handleError();
  }
  parseArguments(argc, argv);
  return 0;
}