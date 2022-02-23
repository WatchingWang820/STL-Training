#pragma once

//https://blog.csdn.net/qq_41614638/article/details/106385037?spm=1001.2014.3001.5501
#include <math.h>
#include <iostream>
#include <vector>
using std::vector;

struct coordinate {
	int x;
	int y;
	bool visited;

	explicit coordinate(int iX = 0, int iY = 0, bool bVisted = false)
		: x(iX), y(iY), visited(bVisted) {
	};

	int operator-(const coordinate& coord)
	{
		return sqrt(pow(x - coord.x, 2) + pow(y - coord.y, 2));
	}
};


class Graph
{
public:
	void PushCoordinate(coordinate* coord)
	{
		m_graph.push_back(coord);
	}

	void PushCoordinate(std::initializer_list<coordinate*> initialList)
	{
		for (auto& i : initialList)
			PushCoordinate(i);
	}

	void PushDestination(coordinate* coord)
	{
		m_dest = coord;
	}

	void SetRadiu(int radius)
	{
		m_radius = radius;
	}

	void IfReachDest(coordinate* start)
	{
		if (Dfs(start))
			std::cout << "Reach" << std::endl;
		else
			std::cout << "Can't reach" << std::endl;
	}

private:
	vector<coordinate*> GetAvailReachCoordinate(coordinate* coord)
	{
		vector<coordinate*> reachCoordinate;
		for (auto& temp : m_graph)
		{
			if (!temp->visited && *temp - *coord < m_radius)
				reachCoordinate.push_back(temp);
		}
		return reachCoordinate;
	}

	bool ReachDest(coordinate* coord)
	{
		if (*coord - *m_dest < m_radius)
			return true;
		else
			return false;
	}

	bool Dfs(coordinate* coord)
	{
		if (ReachDest(coord))
			return true;
		vector<coordinate*> availReachCoord = GetAvailReachCoordinate(coord);
		for (auto& temp : availReachCoord)
		{
			temp->visited = true;
			bool bReach = Dfs(temp);
			if (bReach)
				return true;
		}
		return false;
	}
private:
	vector<coordinate*> m_graph;
	coordinate* m_dest;
	int m_radius;
};
