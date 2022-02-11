#pragma once
namespace watching {

template <typename T>
class default_deleter {
public:
	void operator()(T* ptr) { delete ptr; }
};

}
