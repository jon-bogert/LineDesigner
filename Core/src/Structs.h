#pragma once

struct Connection
{
	int a, b;

	bool Has(int i)
	{
		return i == a || i == b;
	}

	bool Is(int i, int j)
	{
		return (i == a && j == b) || (i == b && j == a);
	}
};