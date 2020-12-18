#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

// GLOBALS
double WINDOWMAP[1][1000][4];
uint32_t OBJHDRSIZE;
uint16_t ROWHDRSIZE;
uint16_t SECTIONHDRSIZE;
uint16_t STREAMVALID;
double HSV_RANGES[][2] = {0.45, 0.66, 0.7, 1, 0.5, 1};
bool is_connected = false;

struct Ellipse
{
	double x = 0;
	double y = 0;
	double theta = 0;
	double w = 0;
	double l = 0;
	double x1 = 0;
	double y1 = 0;
	double x2 = 0;
	double y2 = 0;
};
struct moments
{
	float m00 = 0;
	float m10 = 0;
	float m01 = 0;
	float m11 = 0;
	float m02 = 0;
	float m20 = 0;
};

/* functions definitions
for the time being, i will avoid calling these
functions through the head
*/

bool calcMask(double *HSV, double HSV_RANGES[][2]);
Ellipse calculate_ellipse();
double check_connected(Ellipse e, double base_value);
void process_object(uint8_t serialized_object_data[][4000]);
void label_window(int burstpos, uint16_t wdwidx, int east, Mat img);
void RGB2HSV(double R, double G, double B, double *HSV);
void serialize_object();
void get_scan(Mat img, double array[][2], double WINDOWMAP[1][1000][4]);
void image_read();
int uint2array(int id, int size, int array[]);

int main(int argc, char **argv)
{
	Mat instream;
	instream = imread("backside1.jpg", IMREAD_COLOR);

	OBJHDRSIZE = 8;
	ROWHDRSIZE = 2;
	SECTIONHDRSIZE = 4;

	uint16_t Connected_grains = 0;
	//	uint16_t NUM_ROWS = 1257;
	//	uint16_t NUM_COLS = 901;

	int rows, scan_width, channels;
	rows = instream.rows;
	scan_width = instream.cols;
	channels = instream.channels();


	if (!rows)
	{
		STREAMVALID = 0;
	}

	uint16_t rowcount = 1;
	uint16_t wndwidx = 1;
	uint16_t label_index = 0;

	for (size_t k = 0; k < channels + 1; k++)
	{
		for (size_t j = 0; j < 1000; j++)
		{
			WINDOWMAP[0][j][k] = 0;
		}
	}

	get_scan(instream, HSV_RANGES, WINDOWMAP);

	// testing the values of WINDOWMAP
	// finding only the non-zero indices

	for (size_t i = 0; i < 1000; i++)
	{
		if (WINDOWMAP[0][i][3] != 0)
		{
			cout<<i<<" ";
		}
		
	}
	

	bool isempty = false;	int itr = 0; //temporary

	while (itr<300)
	{

		if (isempty)
		{
			get_scan(instream, HSV_RANGES, WINDOWMAP);
			if (STREAMVALID == 0)
			{
				cout << "Stream ended .." << endl;
			}
			else
			{
				wndwidx = 0;
			}
		}

		for (size_t col = 0; col < scan_width; col++)
		{
			if (WINDOWMAP[wndwidx][col][3] == 1)
			{
				int east = col;
				int burstpos = col;
				label_index += 1;

				while ((east < scan_width) && (WINDOWMAP[wndwidx][east + 1][3] == 1))
				{
					east += 1;
				}

				label_window(burstpos, wndwidx, east, instream);
				// 			// serialize_object()
				// 			// calculate_ellipse()

				// 			// deserialize_object();
				// 			// check_connected;

				// 			// separate_connected_grains();
			}

			// 		// process_object();
			
		}
		itr++; isempty = true; // temporary
		// for (size_t k = 0; k < channels; k++)
		// {
		// 	for (size_t j = 0; j < scan_width; j++)
		// 	{
		// 		WINDOWMAP[wndwidx][j][k] = 0;
		// 	}
		// }

		// int wdw_height = sizeof(WINDOWMAP) / (sizeof(WINDOWMAP[1]));
		// if ((STREAMVALID == 0) && (wdw_height <= 1))
		// {
		// 	break;
		// }
		// rowcount++;
	}
}

bool calcMask(double *HSV, double ranges[][2])
{

	double H, S, V, hmin, hmax, smin, smax, vmin, vmax;
	bool Hbinary, Sbinary, Vbinary, BW;

	H = HSV[0];
	S = HSV[1];
	V = HSV[2];

	// Get thresholds for Hue.
	hmin = ranges[0][0];
	hmax = ranges[0][1];

	// Get thresholds for Saturation
	smin = ranges[1][0];
	smax = ranges[1][1];

	// Get thresholds for Value
	vmin = ranges[2][0];
	vmax = ranges[2][1];

	// Create mask based on thresholds
	Hbinary = (H >= hmin) & (H <= hmax);
	Sbinary = (S >= smin) & (S <= smax);
	Vbinary = (V >= vmin) & (V <= vmax);
	BW = Hbinary & Sbinary & Vbinary;

	// cout<<"value of BW = "<<BW<<endl;
	return BW;
}
Ellipse calculate_ellipse()
{
	moments M;
	Ellipse E;
	float a, b, c, d;

	E.x = M.m10 / M.m00;
	E.y = M.m01 / M.m00;

	a = M.m20 / M.m00 - (pow(E.x, 2));
	b = 2 * (M.m11 / M.m00 - ((E.x) * (E.y)));
	c = M.m02 / M.m00 - (pow(E.y, 2));

	E.theta = 1 / 2 * atan(b / (a - c)) + (a < c) * 3.14 / 2.0;
	E.w = sqrt(8 * (a + c - sqrt((pow(b, 2)) + pow((a - c), 2)))) / 2.0;
	E.l = sqrt(8 * (a + c + sqrt((pow(b, 2)) + pow((a - c), 2)))) / 2.0;

	d = sqrt(pow(E.l, 2) - pow(E.w, 2));
	E.x1 = E.x + d * cos(E.theta);
	E.y1 = E.y + d * sin(E.theta);
	E.x2 = E.x - d * cos(E.theta);
	E.y2 = E.y - d * sin(E.theta);

	return E;
}
double check_connected(Ellipse E, double base_size = 0.00)
{
	static int ave_cnt = 0, CMA_ave = 0;
	int MIN_AVE_COUNT = 5;
	int ave_value, ellipse_size;

	if (!CMA_ave)
	{
		ave_cnt = 0;
		CMA_ave = 0;
	}

	ellipse_size = E.l;

	if (base_size == 0.00)
	{
		if (ave_cnt < MIN_AVE_COUNT)
		{
			is_connected = false;
			CMA_ave = CMA_ave + ((ellipse_size - CMA_ave) / (ave_cnt + 1));
			ave_cnt++;
		}
		else
		{
			if (ellipse_size >= (CMA_ave * 1.6))
			{
				is_connected = true;
			}
			else
			{
				CMA_ave = CMA_ave + (ellipse_size - CMA_ave) / (ave_cnt + 1);
				is_connected = false;
				ave_cnt++;
			}
		}

		ave_value = CMA_ave;
	}
	else
	{
		if (ellipse_size >= base_size * 1.6)
		{
			is_connected = true;
		}
		else
		{
			is_connected = false;
		}
		ave_value = base_size;
	}

	return ave_value;
}
void process_object(uint8_t serialized_object_data[][3080])
{
	uint8_t tempdata;
	// size of serialized_object_data should be sorted
	// serialized_object_data should be a pointer as well!
	tempdata = serialized_object_data[0][0] + 1;
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
		int out_moments = moments;
		int width = (most_right - most_left) + 1;
	}
}
void RGB2HSV(double R, double G, double B, double *HSV)
{
	double var_max, var_min, del_max, del_R, del_G, del_B;
	double H, S, V;
	double var_R = R / 255.0;
	double var_G = G / 255.0;
	double var_B = B / 255.0;

	// finding the maximum
	if ((var_R >= var_G) && (var_R >= var_B))
	{
		var_max = var_R;
	}
	else if ((var_G >= var_R) && (var_G >= var_B))
	{
		var_max = var_G;
	}
	else if ((var_B >= var_R) && (var_G >= var_G))
	{
		var_max = var_B;
	}

	//finding the minimum
	if ((var_R <= var_G) && (var_R <= var_B))
	{
		var_min = var_R;
	}
	else if ((var_G <= var_R) && (var_G <= var_B))
	{
		var_min = var_G;
	}
	else if ((var_B <= var_R) && (var_G <= var_G))
	{
		var_min = var_B;
	}

	del_max = var_max - var_min;

	V = var_max;
	if (del_max == 0)
	{
		H = 0;
		S = 0;
	}
	else
	{
		S = del_max / var_max;
		del_R = (((var_max - var_R) / 6) + (del_max / 2)) / del_max;
		del_G = (((var_max - var_G) / 6) + (del_max / 2)) / del_max;
		del_B = (((var_max - var_B) / 6) + (del_max / 2)) / del_max;

		if (var_R == var_max)
		{
			H = del_B - del_G;
		}
		else if (var_G == var_max)
		{
			H = 0.3334 + del_R - del_B;
		}
		else if (var_B == var_max)
		{
			H = 0.6667 + del_G - del_R;
		}

		if (H < 0)
		{
			H = H + 1;
		}
		if (H > 1)
		{
			H = H - 1;
		}
	}

	HSV[0] = H;
	HSV[1] = S;
	HSV[2] = V;
}
void serialize_object()
{
	double inimage[][901][4] = {};
	double row_ends[][2] = {};
	double left_position, width;

	uint32_t ID = 0;
	double data_ptr = OBJHDRSIZE + ROWHDRSIZE + SECTIONHDRSIZE + 1;

	double num_rows = sizeof(row_ends) / sizeof(row_ends[0]);
	int out_size = (num_rows * width) + (num_rows * 20);
	int outdata[][out_size] = {};

	// need of initializing outdata to zero?

	int hdr_id[4];
	uint2array(ID, 4, hdr_id);
	for (size_t i = 0; i < 4; i++)
	{
		outdata[i][0] = hdr_id[i];
	}

	int hdr_num_rows[2];
	uint2array(num_rows, 2, hdr_num_rows);
	for (size_t i = 0; i < 2; i++)
	{
		outdata[i + 4][0] = hdr_num_rows[i];
	}

	int hdr_width[2];
	uint2array(width, 2, hdr_width);
	for (size_t i = 0; i < 2; i++)
	{
		outdata[i + 6][0] = hdr_width[i];
	}

	int num_sections = 1;
	int section_size = 0;

	int rowhdrptr = OBJHDRSIZE + 1;
	int sectionhdrptr = OBJHDRSIZE + ROWHDRSIZE + 1;

	for (size_t row = 0; row < num_rows; row++)
	{
		int instartcol = row_ends[row][0];
		int shift = row_ends[row][0] - left_position;
		int num_cols = 1 + (row_ends[row][1] - row_ends[row][0]);

		int idx = instartcol;
		while (idx < (instartcol + num_cols))
		{
			uint16_t sectionpos = (idx - instartcol + shift + 1);
			while ((idx < (instartcol + num_cols)) && (inimage[row][idx][4] != 0))
			{
				for (size_t i = data_ptr; i < data_ptr + 2; i++)
				{
					num_sections += 1;
					section_size = 0;
					sectionhdrptr = data_ptr;
					data_ptr = SECTIONHDRSIZE + sectionhdrptr;
					while (inimage[row][idx][4] == 0)
					{
						idx += 1;
					}
				}
			}
		}
		int row_hdr_array[2];
		uint2array(ID, 2, row_hdr_array);
		for (size_t i = 0; i < 2; i++)
		{
			outdata[rowhdrptr + i][0] = row_hdr_array[i];
		}

		rowhdrptr = data_ptr;
		sectionhdrptr = ROWHDRSIZE + rowhdrptr;
		data_ptr = SECTIONHDRSIZE + sectionhdrptr;

		num_sections = 1;
		section_size = 0;
	}
}
void get_scan(Mat img, double array[][2], double WINDOWMAP[1][1000][4])
{
	// cout << "inside get_scan()" << endl;
	double *HSV = new double[3];
	static int imgidx = 0;
	if (imgidx == 0)
	{
		imgidx = 1;
	}

	// cout << "value of imgidx = " << imgidx << endl;
	int rows = img.rows;
	int cols = img.cols;

	Mat planes[3];
	split(img, planes);
	double R, G, B;
	bool bg;

	if (rows < imgidx)
	{
		STREAMVALID = 0;
		imgidx = 0;
	}
	else
	{
		for (int col = 0; col < cols; col++)
		{

			R = planes[2].data[(imgidx-1)*cols + col];
			G = planes[1].data[(imgidx-1)*cols + col];
			B = planes[0].data[(imgidx-1)*cols + col];

			RGB2HSV(R, G, B, HSV);

			bg = calcMask(HSV, HSV_RANGES);

			if (!bg)
			{
				WINDOWMAP[0][col][0] = planes[2].data[(imgidx-1)*cols + col];
				WINDOWMAP[0][col][1] = planes[1].data[(imgidx-1)*cols + col];
				WINDOWMAP[0][col][2] = planes[0].data[(imgidx-1)*cols + col];
				WINDOWMAP[0][col][3] = 1;
			}
		}
		STREAMVALID = 1;
		imgidx += 1;
	}
}
void image_read()
{
	FILE *pf = fopen("beans_text1.txt", "rb");
	long lsize;
	fseek(pf, 0, SEEK_END);
	lsize = ftell(pf);
	rewind(pf);
	cout << lsize << endl;

	uint8_t *buffer;
	buffer = (uint8_t *)malloc(sizeof(uint8_t) * lsize);

	size_t result;
	result = fread(buffer, 1, lsize, pf);
	fclose(pf);

	int num_rows = 309;
	int num_cols = 116;
}
int uint2array(int id, int size, int array[])
{
	array[size] = {};
	// need to make this function
}

// g++ test.cpp -o testoutput -std=c++11 `pkg-config --cflags --libs opencv`
// ./testoutput