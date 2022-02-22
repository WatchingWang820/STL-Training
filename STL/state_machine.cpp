#include "state_machine.h"

std::pair<Action, std::pair<State, State>> OpenAction = {
    Action::OPEN, {State::IDLE, State::OPENED}};
std::pair<Action, std::pair<State, State>> PlayAction = {
    Action::PLAY, {State::OPENED, State::PLAYING}};
std::pair<Action, std::pair<State, State>> PauseAction = {
    Action::PAUSE, {State::PLAYING, State::PAUSING}};
std::pair<Action, std::pair<State, State>> SeekAction = {
    Action::SEEK, {State::OPENED, State::PLAYING}};
std::pair<Action, std::pair<State, State>> StopAction = {
    Action::STOP, {State::PLAYING, State::IDLE}};

std::unordered_map<
    State,
    std::unordered_set<std::pair<Action, std::pair<State, State>>, pair_hash>>
    MediaPlayerStateMap = {
        {State::IDLE, {OpenAction}},
        {State::OPENED, {PlayAction, SeekAction, StopAction}},
        {State::PLAYING, {PauseAction, SeekAction, StopAction}},
        {State::PLAYED, {SeekAction, StopAction}},
        {State::PAUSING, {PlayAction, SeekAction, StopAction}}};