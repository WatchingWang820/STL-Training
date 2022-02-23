#pragma once
#include <vector>
#include <iostream>

template <typename T>
class BigHeapOrder
{
public:
	bool operator()(T val1, T val2)
	{
		return val1 > val2;
	}
};

template <typename T>
class SmallHeapOrder
{
public:
	bool operator()(T val1, T val2)
	{
		return val1 < val2;
	}
};

template <typename T, typename Order>
class Heap
{
public:
	Heap() {
		///放个哨兵对数组索引有帮助，并减少判断，但对类型固定成int了，有待改善
		if (typeid(Order) == typeid(BigHeapOrder<T>))
			m_vec.push_back(INT_MAX);
		else
			m_vec.push_back(INT_MIN);
	}

public:
	void InsertHeap(T value);
	T DeleteTop();
	void PrintHeap();

private:
	std::vector<T> m_vec;
};

template <typename T, typename Order>
void Heap<T, Order>::InsertHeap(T value)
{
	int index = m_vec.size();
	m_vec.push_back(value);

	for (; Order()(value, m_vec[index / 2]); index /= 2)
		m_vec[index] = m_vec[index / 2];

	m_vec[index] = value;
	++index;
}

template <typename T, typename Order>
T Heap<T, Order>::DeleteTop()
{
	if (m_vec.size() <= 1)
		return NULL;

	T top = m_vec[1];
	T temp = m_vec[m_vec.size() - 1];
	m_vec.pop_back();

	int Parent, Child;
	for (Parent = 1; Parent * 2 < m_vec.size(); Parent = Child)
	{
		Child = 2 * Parent;
		if (Child != m_vec.size() &&
			Order()(m_vec[Child + 1], m_vec[Child]))
			++Child;
		if (Order()(m_vec[Child], temp))
			m_vec[Parent] = m_vec[Child];
		else
			break;
	}

	m_vec[Parent] = temp;
	return top;
}

template <typename T, typename Order>
void Heap<T, Order>::PrintHeap()
{
	using std::cout;
	for (auto& i : m_vec)
		cout << i << ' ';
}
