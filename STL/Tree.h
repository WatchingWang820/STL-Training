#pragma once

#include <iostream>
// ����ͼ�д������ģ����
int preArray[] = { 1,2,3,4,5,6 };
int inArray[] = { 3,2,4,1,6,5 };
int postArray[6] = { 0 };

//����ǰ�������һ��λ�ã�Ϊ���ڵ㣬����������ڵ�����Ϊ���������Ҳ�Ϊ���������ݹ���ν����
void createPost(int preL, int inL, int postL, int n)
{
	if (n == 0) return;
	if (n == 1)
	{
		postArray[postL] = preArray[preL];
		return;
	}

	int root = preArray[preL];
	postArray[postL + n - 1] = root;
	int i = 0;
	for (; i < n; i++)
	{
		if (inArray[inL + i] == root)
			break;
	}

	int Ln = i;
	int Rn = n - Ln - 1;
	createPost(preL + 1, inL, postL, Ln);
	createPost(preL + Ln + 1, inL + Ln + 1, postL + Ln, Rn);
}

void main()
{
	createPost(0, 0, 0, 6);
	for (auto& temp : postArray)
	{
		std::cout << temp << ' ';
	}
}
