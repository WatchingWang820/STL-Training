#pragma once
#pragma once
#include <iostream>;

namespace Sort
{
	template <typename T>
	class BigOrder
	{
	public:
		bool operator()(T val1, T val2)
		{
			return val1 > val2;
		}
	};

	template <typename T>
	class SmallOrder
	{
	public:
		bool operator()(T val1, T val2)
		{
			return val1 < val2;
		}
	};

	template <typename T>
	static void swap(T& a, T& b)
	{
		T temp = b;
		b = a;
		a = temp;
	}

	template <typename T, typename Order>
	void BubbleSort(T array[], Order, int n)
	{
		for (int i = n - 1; i >= 0; i--)
		{
			for (int j = 0; j < i; j++)
			{
				if (Order()(array[j], array[j + 1]))
					swap(array[j], array[j + 1]);
			}
		}
	}

	template <typename T, typename Order>
	void InsertSort(T array[], Order, int n)
	{
		for (int i = 1; i < n; i++)
		{
			T temp = array[i];
			int j;
			for (j = i; j > 0 && Order()(array[j - 1], temp); j--)
				array[j] = array[j - 1];
			array[j] = temp;
		}
	}

	template <typename T, typename Order>
	void QuickSort(T array[], Order order, int left, int right)
	{
		if (left >= right)
			return;

		int i = left;
		int j = right;
		T pivot = array[right];

		while (1)
		{
			while (array[i] <= pivot && i < j)
				++i;
			while (array[j] >= pivot && i < j)
				--j;

			if (i < j)
				swap(array[i], array[j]);
			else
				break;
		}
		swap(array[right], array[i]);
		QuickSort(array, order, left, i - 1);
		QuickSort(array, order, i + 1, right);
	}

	template <typename T, typename Order>
	void ShellSort(T array[], Order, int n)
	{
		for (int i = n / 2; i > 0; i /= 2)
		{
			for (int j = i; j < n; j++)
			{
				T temp = array[j];
				int k;
				for (k = j; k >= i && Order()(array[k - i], temp); k -= i)
					array[k] = array[k - i];
				array[k] = temp;
			}
		}
	}

	template <typename T, typename Order>
	void TableSort(T array[], Order, int n)
	{
		int* table = new int[n];
		for (int i = 0; i < n; i++)
			table[i] = i;

		for (int i = 1; i < n; i++)
		{
			T temp = table[i];
			int j = i;
			for (j; j > 0 && Order()(array[table[j - 1]], array[temp]); j--)
				table[j] = table[j - 1];
			table[j] = temp;
		}
		for (int i = 0; i < n; i++)
		{
			std::cout << table[i] << ' ';
		}
	}
}
