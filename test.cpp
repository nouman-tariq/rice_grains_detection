#include <iostream>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

double WINDOWMAP[1][1000][4];
double HSV_RANGES[][2] = {0.45, 0.66, 0.7, 1, 0.5, 1};
struct moments
{
	float m00 = 0;
	float m10 = 0;
	float m01 = 0;
	float m11 = 0;
	float m02 = 0;
	float m20 = 0;
};
bool STREAMVALID;


void label_window(int burstpos, uint16_t wdwidx, int east, Mat img);

int main()
{
	Mat instream = imread("backside1.jpg");
	int burstpos = 207;
	uint16_t wndwidx = 1;
	int east = 215;
	STREAMVALID = 1;
	

	// initializing WINDOWMAP to 0
	for (size_t k = 0; k < 4; k++)
	{
		for (size_t j = 0; j < 1000; j++)
		{
			WINDOWMAP[0][j][k] = 0;
		}
	}

	//initializing WINDOWMAP to step values
	for (size_t i = burstpos - 1; i < east; i++)
	{
		WINDOWMAP[0][i][3] = 1;
	}
	

	label_window(burstpos, wndwidx, east, instream);


}

void label_window(int burstpos, uint16_t wdwidx, int east, Mat img)
{
	static int most_left, recursion_cnt;
	int section_start, instream, hsv_ranges, most_right;
	static int object_edges[][2] = {};
	int row, col;

	if (!recursion_cnt)
	{
		recursion_cnt = 1;
		object_edges[0][0] = {section_start};
		object_edges[0][1] = {col};
		most_left = section_start;
		most_right = col;
	}
	else
	{
		recursion_cnt += 1;
	}
	int height;
	height = sizeof(object_edges);

	if (height < row)
	{
		object_edges[row][1] = {col};
		object_edges[row][2] = {col};
	}

	if (col > object_edges[row][2])
	{
		object_edges[row][2] = col;
		most_right = col;
	}

	while ((col > 1) && (WINDOWMAP[row][col - 1][4] == 1))
	{
		col = col - 1;
	}

	int m = col;

	if (col < object_edges[row][0])
	{
		object_edges[row][0] = {col};
	}

	// size of WINDOWMAP
	int scan_width = sizeof(WINDOWMAP) / sizeof(WINDOWMAP[row][col - 1]);

	if ((m <= scan_width) && (WINDOWMAP[row][m][4]) == 1)
	{
		if (m < most_left)
		{
			most_left = m;
		}

		int data_valid = 1;
		while (data_valid == 1)
		{
			// update_moments(moments, row, m);
			WINDOWMAP[row][m][4] = 2;

			if (((m + 1) > scan_width) || (WINDOWMAP[row][m + 1][4] != 1))
			{
				data_valid = 0;
				if (object_edges[row][2] < m)
				{
					object_edges[row][2] = m;
				}
			}
			m = m + 1;
		}
	}

	//12:30

	while ((col < m) && (col > 0) && (col <= scan_width))
	{
		if ((row > 1) && (WINDOWMAP[row - 1][col][4] == 1))
		{
			// label_window(section_start, row-1, col, instream, hsv_ranges);
		}

		int wrows = sizeof(WINDOWMAP) / sizeof(WINDOWMAP[1]);
		if ((wrows == row) && (STREAMVALID == 1))
		{
			// = get_scan(instream, hsv_ranges);
		}

		if (WINDOWMAP[row + 1][col][4] == 1)
		{
			// label_window(section_start, row-1, col, instream, hsv_ranges);
		}

		col = col + 1;
	}

	recursion_cnt -= 1;
	if (recursion_cnt == 0)
	{
		recursion_cnt = {};
		int obj_edges = object_edges[0][2];
		int left_position = most_left;
		// int out_moments = moments;
		int width = (most_right - most_left) + 1;
	}
}
