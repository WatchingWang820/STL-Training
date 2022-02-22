#pragma once
#include <array>
#include <functional>

template <typename T, int Capcity = 10>
struct RingArray {
    T* PeekReadableData() {
        if (valid_data_count_ == 0) {
            return nullptr;
        }

        return &data_array_[read_index_];
    }

    void SetMoveFunc(std::function<void(T*, T*)> move_func) {
        move_type_func_ = std::move(move_func);
    }

    void HasReadOneData() {
        --valid_data_count_;
        read_index_ = (read_index_ + 1) % Capcity;
    }

    bool HasEnoughData() {
        if (valid_data_count_ == Capcity) {
            return true;
        }
        else {
            return false;
        }
    }

    void WriteData(T* data) {
        if (valid_data_count_ == Capcity) {
            return;
        }

        if (move_type_func_) move_type_func_(&data_array_[write_index_], data);
        write_index_ = (write_index_ + 1) % Capcity;
        ++valid_data_count_;
    }

private:
    size_t valid_data_count_ = 0;
    size_t write_index_ = 0;
    size_t read_index_ = 0;
    std::function<void(T*, T*)> move_type_func_;
    std::array<T, Capcity> data_array_;
};