#pragma once
template <typename T>
class more
{
public:
	bool operator()(T a, T b) {
		return a > b;
	}
};

//less 举一反三写，用于数组是递减情况

template <typename T, typename order>
int BinarySearch(T array[], T find, int left, int right, order)
{
	if (left > right)
		return -1;

	int mid = (left + right) / 2;
	if (array[mid] == find)
		return mid;
	else if (order()(find, array[mid]))
		return BinarySearch(array, find, mid + 1, right, order());
	else
		return BinarySearch(array, find, left, mid - 1, order());
}

typedef bool(*orderFunc)(int, int); //模板实例化，处理传lambada函数
template <> int BinarySearch<int, orderFunc>(int array[], int find, int left, int right, orderFunc func)
{
	if (left > right)
		return -1;

	int mid = (left + right) / 2;
	if (array[mid] == find)
		return mid;
	else if (func(find, array[mid]))
		return BinarySearch(array, find, mid + 1, right, func);
	else
		return BinarySearch(array, find, left, mid - 1, func);
}

void main()
{
	int array[] = { 1,2,3,4,5 };
	int index = BinarySearch(array, 4, 0, 4, more<int>());
	orderFunc func = [](int a, int b)->bool {return a > b; };
	int index2 = BinarySearch(array, 4, 0, 4, func);
}
