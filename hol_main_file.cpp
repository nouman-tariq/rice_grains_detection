#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;


// GLOBALS
uint16_t WINDOWMAP[116][309][4];
int row_ends[100][2];
uint16_t OBJHDRSIZE = 0;
uint16_t ROWHDRSIZE = 0;
uint16_t SECTIONHDRSIZE = 0;
uint16_t STREAMVALID = 0;
uint16_t left_position = 0;
uint16_t width = 0;
float HSV_RANGES[][2] = {0.45, 0.66, 0.7, 1, 0.5, 1};
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
in the final code, a separate header file can used for the 
following functions.
*/

bool calcMask(float *HSV);
Ellipse calculate_ellipse(moments &mm);
double check_connected(Ellipse e, double base_value);
void process_object(uint8_t serialized_object_data[][4000]);
moments label_window(int burstpos, int wdwidx, int east, Mat img);
void RGB2HSV(double R, double G, double B, double *HSV);
uint8_t * serialize_object();
void get_scan(Mat img, int wdw_row);
void image_read();
void uint2array(uint32_t ID, int numbytes, uint8_t *hdr_id);
void update_moments(moments &M, int row, int col);
void uint2array(uint32_t ID, int numbytes, uint8_t hdr_id);



int main(int argc, char **argv)
{
	Mat instream;
	instream = imread("backside1.jpg", IMREAD_COLOR);

	moments mm;
	Ellipse ellipse[1000]; // temporarily setting the structure member size, need to find the permanent solution later.
	OBJHDRSIZE = 8;
	ROWHDRSIZE = 2;
	SECTIONHDRSIZE = 4;

	uint16_t Connected_grains = 0;
	//	uint16_t NUM_ROWS = 1257;
	//	uint16_t NUM_COLS = 901;
	uint8_t *outobject;

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

	get_scan(instream, wndwidx);

	bool isempty = false;
	int itr = 0; //temporary

	while (itr<300)
	{

		if (isempty)
		{
			get_scan(instream, 0);
			if (STREAMVALID == 0)
			{
				cout << "Stream ended .." << endl;
			}
			else
			{
				wndwidx = 1;
			}
		}

		for (size_t col = 0; col < scan_width; col++)
		{
			if (WINDOWMAP[wndwidx - 1][col][3] == 1)
			{
				int east = col;
				int burstpos = col;

				while ((east < scan_width) && (WINDOWMAP[wndwidx - 1][east + 1][3] == 1))
				{
					east += 1;
				}

				mm = label_window(burstpos, wndwidx, east, instream);
				outobject = serialize_object();
				ellipse[label_index] = calculate_ellipse(mm);
				ellipse[label_index].y = (ellipse[label_index].y + rowcount) - 1;
				label_index += 1;

				// 			// deserialize_object();
				// 			// check_connected;
				// 			// separate_connected_grains();
			}

			// 		// process_object();
		}
		itr++;

		int wdw_height, temp;
		for (size_t i = 0; i < rows; i++)
		{
		for (size_t j = 0; j < scan_width; j++)
			{
				temp++;
				if (WINDOWMAP[i][j][3] != 0)
				{
					break;
				}

				if (temp == scan_width-1)
				{
					wdw_height = i-1;
				}
				
				
			}
			temp = 0;
			if (wdw_height>0)
			{
				break;
			}
			
		}

		for (size_t k = 0; k < channels; k++)
		{
			for (size_t j = 0; j < scan_width; j++)
			{				
				for (size_t i = 0; i < rows; i++)
				{
					if (WINDOWMAP[i][j][k] != 0)
					{
						isempty = false;
						break;
					}
					else
					{
						isempty = true;
					}
					
					
				}
				if (!isempty)
				{
					break;
				}
					
				
			}
			if(!isempty)
			{
				break;
			}
		}

		if ((STREAMVALID == 0) && (wdw_height < 1))
		{
			break;
		}
		rowcount++;
	}
}




bool calcMask(float *HSV)
{

	double H, S, V, hmin, hmax, smin, smax, vmin, vmax;
	bool Hbinary, Sbinary, Vbinary, BW;

	H = HSV[0];
	S = HSV[1];
	V = HSV[2];

	// Get thresholds for Hue.
	hmin = HSV_RANGES[0][0];
	hmax = HSV_RANGES[0][1];

	// Get thresholds for Saturation
	smin = HSV_RANGES[1][0];
	smax = HSV_RANGES[1][1];

	// Get thresholds for Value
	vmin = HSV_RANGES[2][0];
	vmax = HSV_RANGES[2][1];

	// Create mask based on thresholds
	Hbinary = (H >= hmin) & (H <= hmax);
	Sbinary = (S >= smin) & (S <= smax);
	Vbinary = (V >= vmin) & (V <= vmax);
	BW = Hbinary & Sbinary & Vbinary;

	// cout<<"value of BW = "<<BW<<endl;
	return BW;
}
Ellipse calculate_ellipse(moments &M)
{
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
moments label_window(int section_start, int row, int col, Mat instream)
{
	static int most_left, recursion_cnt;
	int most_right;
	static int object_edges[100][2] = {};
	static moments M;

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
	for (size_t i = 0; i < 100; i++)
	{
		if (	(object_edges[i][0] && object_edges[i][1]) == 0	)
		{
			height = i;
			break;
		}
		
	}
	

	if (height < row)
	{
		object_edges[row - 1][0] = {col};
		object_edges[row - 1][1] = {col};
	}

	if (col > object_edges[row - 1][1])
	{
		object_edges[row - 1][1] = col;
		most_right = col;
	}

	while ((col > 1) && (WINDOWMAP[row - 1][col - 1][3] == 1))
	{
		col = col - 1;
	}

	int m = col;

	if (col < object_edges[row - 1][0])
	{
		object_edges[row - 1][0] = {col};
	}

	// int scan_width = sizeof(WINDOWMAP) / sizeof(WINDOWMAP[row-1][col - 1]);
	int scan_width = instream.cols;

	if ((m <= scan_width-1) && (WINDOWMAP[row - 1][m][3]) == 1) // need to verify the changes
	{
		if (m < most_left)
		{
			most_left = m;
		}

		int data_valid = 1;
		while (data_valid == 1)
		{
			update_moments(M, row, col); 
			WINDOWMAP[row - 1][m][3] = 2;

			if (((m + 1) > scan_width) || (WINDOWMAP[row - 1][m + 1][3] != 1))
			{
				data_valid = 0;
				if (object_edges[row - 1][1] < m)
				{
					object_edges[row - 1][1] = m;
				}
			}
			m = m + 1;
		}
	}

	while ((col < m) && (col > 0) && (col < scan_width))
	{
		if ((row > 1) && (WINDOWMAP[row - 2][col][3] == 1))
		{
			label_window(section_start, row - 1, col, instream);
		}

		int total_rows = sizeof(WINDOWMAP) / sizeof(WINDOWMAP[0]);
		int wrows = 0, temp =0;
		// adding a loop to figure out the wrows
		for (size_t i = 0; i < total_rows; i++)
		{
			for (size_t j = 0; j < scan_width; j++)
			{
				temp++;
				if (WINDOWMAP[i][j][3] != 0)
				{
					break;
				}

				if (temp == scan_width)
				{
					wrows = i;
				}
				
				
			}
			temp = 0;
			if (wrows>0)
			{
				break;
			}
			
		}
		

		if ((wrows == row) && (STREAMVALID == 1))
		{
			get_scan(instream, row + 1); 
		}

		if (WINDOWMAP[row][col][3] == 1)
		{
			label_window(section_start, row + 1, col, instream);
		}

		col = col + 1;
	}

	recursion_cnt -= 1;
	if (recursion_cnt == 0)
	{
		recursion_cnt = {};
		left_position = most_left;
		width = (most_right - most_left) + 1;
		for (size_t i = 0; i < 100; i++)
		{
			row_ends[i][0] = object_edges[i][0];
			row_ends[i][1] = object_edges[i][1];
		}
		return M;
	}
}

void RGB2HSV(float R, float G, float B, float *HSV)
{
	float var_max, var_min, del_max, del_R, del_G, del_B;
	float H, S, V;
	float var_R = R / 255.0;
	float var_G = G / 255.0;
	float var_B = B / 255.0;

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
uint8_t * serialize_object()
{
	uint32_t ID = 0;
	int data_ptr = OBJHDRSIZE + ROWHDRSIZE + SECTIONHDRSIZE + 1;

	double temp_num_rows = sizeof(row_ends) / sizeof(row_ends[0]);
	int num_rows = 0, temp =0;
	// adding a loop to figure out the num_rows
	for (size_t i = 0; i < temp_num_rows; i++)
	{
		if ((row_ends[i][0] == 0)&&(row_ends[i][1] == 0))
		{
			num_rows = i;
			break;
		}			
	}

	int out_size = (num_rows * width) + (num_rows * 20);
	static uint8_t *outdata = new uint8_t[out_size*out_size];
	for (size_t i = 0; i < (out_size*out_size); i++)
	{
		outdata[i] = 0;
	}
	

	uint8_t *hdr_id = new uint8_t[4];
	for (size_t i = 0; i < 4; i++)
	{
		hdr_id[i] = 0;
	}		
	uint2array(ID, 4, hdr_id);
	for (size_t i = 0; i < 4; i++)
	{
		outdata[i] = hdr_id[i];
	}


	uint8_t *hdr_num_rows = new uint8_t[2];
	for (size_t i = 0; i < 2; i++)
	{
		hdr_num_rows[i] = 0;
	}	
	uint2array(num_rows, 2, hdr_num_rows);
	for (size_t i = 0; i < 2; i++)
	{
		outdata[4+i] = hdr_num_rows[i];
	}


	uint8_t *hdr_width = new uint8_t[2];
	for (size_t i = 0; i < 2; i++)
	{
		hdr_width[i] = 0;
	}	
	uint2array(width, 2, hdr_width);
	for (size_t i = 0; i < 2; i++)
	{
		outdata[6+i] = hdr_width[i];
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
			while ((idx < (instartcol + num_cols)) && (WINDOWMAP[row][idx][3] != 0))
			{
			 	outdata[data_ptr - 1] = (uint8_t) WINDOWMAP[row][idx][0]; // value of data_ptr is decreased inside image indexing
				outdata[data_ptr + 0] = (uint8_t) WINDOWMAP[row][idx][1];
				outdata[data_ptr + 1] = (uint8_t) WINDOWMAP[row][idx][2];

				data_ptr += 3;
				section_size += 1;
				idx += 1;
			}

			uint8_t *temp_array = new uint8_t[2];
			for (size_t i = 0; i < 2; i++)
			{
				temp_array[i] = 0;
			}	
			uint2array(sectionpos, 2, temp_array);
			for (size_t i = 0; i < 2; i++)
			{
				outdata[sectionhdrptr-1+i] = temp_array[i];
			}

			for (size_t i = 0; i < 2; i++)
			{
				temp_array[i] = 0;
			}	
			uint2array(section_size, 2, temp_array);
			for (size_t i = 2; i < 4; i++)
			{
				outdata[sectionhdrptr-1+i] = temp_array[i-2];
			}

			while ((idx < (instartcol + num_cols)) && (WINDOWMAP[row][idx][3] == 0))
			{
				num_sections += 1;
				section_size = 0;
				sectionhdrptr = data_ptr;
				data_ptr = SECTIONHDRSIZE + sectionhdrptr;
				while (WINDOWMAP[row][idx][3] == 0)
				{
					idx += 1;
				}
			
			}
			
		}

		uint8_t *temp_array = new uint8_t[2];
		for (size_t i = 0; i < 2; i++)
		{
			temp_array[i] = 0;
		}	
		uint2array(num_sections, 2, temp_array);
		for (size_t i = 0; i < 2; i++)
		{
			outdata[rowhdrptr-1+i] = temp_array[i];
		}

		rowhdrptr = data_ptr;
		sectionhdrptr = ROWHDRSIZE + rowhdrptr;
		data_ptr = SECTIONHDRSIZE + sectionhdrptr;
		num_sections = 1;
		section_size = 0;
	
	}
	return outdata;
}
void get_scan(Mat img, int wdw_row)
{

	float *HSV = new float[3];
	static int imgidx = 0;
	if (imgidx == 0)
	{
		imgidx = 1;
	}

	int rows = img.rows;
	int cols = img.cols;

	Mat planes[3];
	split(img, planes);
	float R, G, B;
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

			R = planes[2].data[(imgidx - 1) * cols + col];
			G = planes[1].data[(imgidx - 1) * cols + col];
			B = planes[0].data[(imgidx - 1) * cols + col];

			RGB2HSV(R, G, B, HSV);

			bg = calcMask(HSV);

			if (!bg)
			{
				if (wdw_row > 0)
				{
					WINDOWMAP[wdw_row - 1][col][0] = planes[2].data[(imgidx - 1) * cols + col];
					WINDOWMAP[wdw_row - 1][col][1] = planes[1].data[(imgidx - 1) * cols + col];
					WINDOWMAP[wdw_row - 1][col][2] = planes[0].data[(imgidx - 1) * cols + col];
					WINDOWMAP[wdw_row - 1][col][3] = 1;
				}
				else
				{
					WINDOWMAP[wdw_row][col][0] = planes[2].data[(imgidx - 1) * cols + col];
					WINDOWMAP[wdw_row][col][1] = planes[1].data[(imgidx - 1) * cols + col];
					WINDOWMAP[wdw_row][col][2] = planes[0].data[(imgidx - 1) * cols + col];
					WINDOWMAP[wdw_row][col][3] = 1;
				}
				
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
void update_moments(moments &M, int row, int col)
{
	M.m00 = M.m00 + (pow(col, 0) * pow(row, 0));
	M.m10 = M.m10 + (pow(col, 1) * pow(row, 0));
	M.m01 = M.m01 + (pow(col, 0) * pow(row, 1));
	M.m11 = M.m11 + (pow(col, 1) * pow(row, 1));
	M.m02 = M.m02 + (pow(col, 0) * pow(row, 2));
	M.m20 = M.m20 + (pow(col, 2) * pow(row, 0));
}
void uint2array(uint32_t num, int numbytes, uint8_t *hdr_id)
{
	switch (numbytes)
	{
	case 1:
		uint8_t outcast_8;
		outcast_8 = uint8_t (num);
		hdr_id[0] = (uint8_t) outcast_8;
		break;
	case 2:
		uint16_t outcast_16;
		outcast_16 = uint16_t (num);
		hdr_id[0] = (uint8_t) outcast_16;
		break;
	case 4:
		uint32_t outcast_32;
		outcast_32 = uint32_t (num);
		hdr_id[0] = (uint8_t) outcast_32;
		break;
	case 8:
		uint64_t outcast_64;
		outcast_64 = uint64_t (num);
		hdr_id[0] = (uint8_t) outcast_64;
		break;
	default:
		cout<<"Invalid data type entered- "<<endl;
		break;
	}

}


// g++ -g hol_main_file.cpp -o hol_main_file -std=c++11 `pkg-config --cflags --libs opencv`
// ./testoutput