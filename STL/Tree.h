#pragma once

#include <iostream>
// 这里就简单写个数组模拟树
int preArray[] = { 1,2,3,4,5,6 };
int inArray[] = { 3,2,4,1,6,5 };
int postArray[6] = { 0 };

//根据前序遍历第一个位置，为根节点，中序遍历根节点的左侧为左子树，右侧为右子树，递归分治解决。
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
