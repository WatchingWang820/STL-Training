#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "src/core/GlobalService.h"

enum class State : uint8_t { IDLE, OPENED, PLAYING, PLAYED, PAUSING };

enum class Action : uint8_t { OPEN, PLAY, PAUSE, SEEK, STOP };

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    return h1;
  }
};

extern std::unordered_map<
    State,
    std::unordered_set<std::pair<Action, std::pair<State, State>>, pair_hash>>
    MediaPlayerStateMap;

template <typename ActionType, typename StateType, typename Hash>
class FiniteStateSet {
  using StateSet = std::unordered_map<
      StateType,
      std::unordered_set<std::pair<ActionType, std::pair<StateType, StateType>>,
                         Hash>>;

 public:
  explicit FiniteStateSet(const StateType& initialize_state, StateSet set)
      : initialize_state_(initialize_state),
        current_state_(initialize_state),
        finite_state_set_(set) {}

  StateType GetCurrentState() { return current_state_; }
  void TransferNextState(ActionType action, int excute_result) {
    for (auto& pair : finite_state_set_[current_state_]) {
      if (pair.first == action) {
        excute_result == 0 ? current_state_ = pair.second.second
                           : current_state_ = pair.second.first;
        break;
      }
    }
  }
  bool IsSupportAction(const ActionType& action) {
    for (auto& pair : finite_state_set_[current_state_]) {
      if (pair.first == action) {
        return true;
      }
    }
    return false;
  }

 private:
  const StateType initialize_state_;
  StateType current_state_;
  StateSet finite_state_set_;
};

class StateMachine {
 public:
  explicit StateMachine(
      WorkerType worker,
      std::unique_ptr<FiniteStateSet<Action, State, pair_hash>> set)
      : worker_(worker), state_set_(std::move(set)) {}

  State GetCurrentState() {
    State state;
    worker_->SyncCall([&] {
      state = state_set_->GetCurrentState();
      return 0;
    });
    return state;
  }

  bool IsSupportAction(const Action& action) {
    return worker_->SyncCall(
        [=] { return state_set_->IsSupportAction(action); });
  }

  int DoAction(const Action& action, std::function<int()> func) {
    return worker_->SyncCall([=] {
      if (!IsSupportAction(action)) {
        return -1;
      }

      int ret = func();
      state_set_->TransferNextState(action, ret);
      return ret;
    });
  }

 private:
  WorkerType worker_;
  std::unique_ptr<FiniteStateSet<Action, State, pair_hash>> state_set_;
};
