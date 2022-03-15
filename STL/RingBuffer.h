#pragma once
#include <mutex>
#include <condition_variable>
#include <array>

class RingBuffer {
public:
	int WriteData(unsigned char* buf, int buf_size, bool sync = true);
	int ReadData(unsigned char* buf, int buf_size, bool sync = true);

private:
	static const size_t BUF_SIZE = 32 * 1024;

	size_t valid_buf_count_ = 0;
	size_t cur_read_index_ = 0;
	size_t cur_write_index_ = 0;
	std::mutex buf_mtx_;
	std::condition_variable buf_cv_;
	std::array<unsigned char, BUF_SIZE> buffer_;
};

int RingBuffer::WriteData(unsigned char* buf, int buf_size, bool sync) {
	std::unique_lock<decltype(buf_mtx_)> lck(buf_mtx_);
	if (BUF_SIZE - valid_buf_count_ < buf_size) {
		if (!sync) {
			return -1;
		}

		buf_cv_.wait(lck, [=] { return (BUF_SIZE - valid_buf_count_) >= buf_size; });
	}

	auto write_size = std::min((int)(BUF_SIZE - cur_write_index_), buf_size);
	std::copy(buf, buf + write_size, buffer_.data() + cur_write_index_);
	if (write_size != buf_size) {
		std::copy(buf + write_size, buf + buf_size, buffer_.data());
	}
	cur_write_index_ = (cur_write_index_ + buf_size) % BUF_SIZE;
	valid_buf_count_ += buf_size;
	buf_cv_.notify_one();
	return buf_size;
}

int RingBuffer::ReadData(unsigned char* buf, int buf_size, bool sync) {
	std::unique_lock<decltype(buf_mtx_)> lck(buf_mtx_);
	if (valid_buf_count_ < buf_size) {
		if (!sync) {
			return -1;
		}

		buf_cv_.wait(lck, [=] { return valid_buf_count_ >= buf_size; });
	}

	size_t read_index = cur_read_index_ + buf_size;
	std::copy(buffer_.data() + cur_read_index_, buffer_.data() + std::min(BUF_SIZE, read_index), buf);

	size_t pre_read_index = std::max(0, (int)(read_index - BUF_SIZE));
	if (pre_read_index != 0) {
		std::copy(buffer_.data(), buffer_.data() + pre_read_index, buf + BUF_SIZE - pre_read_index);
	}

	cur_read_index_ = (cur_read_index_ + buf_size) % BUF_SIZE;
	buf_cv_.notify_one();
	return buf_size;
}