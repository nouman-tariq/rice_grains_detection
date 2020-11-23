function HOL_segment_connected_coder_2020_09_24()

%--------------------------------------------------------------------------
% don't need in final application
%--------------------------------------------------------------------------
% close all;
% clear global;
% clear label_window; %clear persistent variables in label_window()
% clearNoBP all; %clear all while saving breakpoints
% clear 
% clc
%--------------------------------------------------------------------------

% Get the initial image
fname = 'beans_test1.jpg';
binfname = 'beans_test1.txt';

%------------------------------
% Data Format
%------------------------------
% Format: [OBJHEADER][ROWHEADER][SUBSECTIONHEADER][...data...][SUBSECTIONHEADER][...data...]
% [OBJHEADER]: [ID], [Num rows], [Object width]
% [ROWHEADER]: [num sections in row]
% [SUBSECTIONHEADER]: [Position of section], [Num pixels in section]

%: ID, num rows in object, object width, num section in row, position of section, num pixels in section, [data...], position, num pixels in row, [data...], ...
%	ID: 8 bytes (4 billion objects)
%	Num rows: 2 bytes (16,500 pixels high max)
%	Object width: 2 bytes (16,500 pixels wide max)
%	Num sections in row: 2 bytes (16,500 sections max)
%	Position of section: 2 bytes (possible furthest position is 16,500)
%	Num pixels in section: 2 bytes (max possible pixels in section/row is 16,500)
%------------------------------

%-----------------------------
% GLOBALS
global WINDOWMAP %used globally can optimize in c++
global OBJHDRSIZE 
global ROWHDRSIZE
global SECTIONHDRSIZE
global STREAMVALID
global HSV_RANGES

%-----------------------------
%CONSTANTS 
%HDRSIZE = 6;  %Header (Old [uint8]-based): ID, num rows in object, object width, num section in row, position of section, num pixels in section, [data...], position, num pixels in row, [data...], ...
OBJHDRSIZE = 8;  %ID [uint32], num rows in object [uint16], object width [uint16],  [data...], position, num pixels in row
ROWHDRSIZE = 2;  %num section in row [uint16]
SECTIONHDRSIZE = 4;  %position of section [uint16], num pixels in section[uint16]
%BASEHDRSIZE = OBJHDRSIZE + ROWHDRSIZE+SECTIONHDRSIZE;  %Header: ID, num rows in object, object width, num section in row, position of section, num pixels in section, [data...], position, num pixels in row, [data...], ...
%-----------------------------

%-----------------------------
Connected_grains = 0; 
%-----------------------------

Ellipse = struct('x', 0, 'y', 0, 'theta', 0, 'w', 0, 'l', 0, 'x1', 0, 'y1', 0, 'x2', 0, 'y2', 0);
%tellipse = struct('x', 13.3, 'y', 1.875, 'theta',0.02, 'w', 1.853, 'l', 3.296, 'x1', 16.037, 'y1', 1.932, 'x2', 10.588, 'y2', 1.818);  %test  ellipse

% instream = imread(fname);

%------------------------------------------------------------------------
% used for saving image data in binary form to avoid Matlab coder warning 
% should use standard C/C++ functions in final application
%------------------------------------------------------------------------
% imwrite_binary(binfname, instream);

NUM_ROWS = 1257;
NUM_COLS = 901;
instream = imread_binary(binfname, NUM_ROWS, NUM_COLS);

%-----------------------------
% for displaying output don't need for final application 
%-----------------------------
% 	s = get(0, 'ScreenSize');
% 	base_fig_handle = figure('Position', [0 0 s(3) s(4)]);
% 	imshow(instream);
% 	set(gcf, 'units','normalized','outerposition',[0 0 1 1]);
% 	hold on
%-----------------------------

[rows, scan_width, channels] = size(instream); %#ok<ASGLU>
if rows == 0
	STREAMVALID = 0;
end

HSV_RANGES = [.45,.66; .7,1; .5,1]; %Hue of beans_testX.png background is 212 degrees so give a range to +10 degrees
	
rowcount = 1;
wndwidx = 1;
label_index = 0; %label
WINDOWMAP = zeros(1, scan_width, 4, 'uint8'); %set to uint8...it automatically was making them doubles

[WINDOWMAP(wndwidx, :,:), STREAMVALID] = get_scan(instream, HSV_RANGES);  %removing this causes an offset for ellipse.  Need to fix.  Bug!!!

while(1)
	if isempty(WINDOWMAP) %for rows without any foreground objects
		[WINDOWMAP, STREAMVALID] = get_scan(instream, HSV_RANGES);
		if(STREAMVALID == 0)
			%error('Stream is not open.  Exiting!');
			disp('Stream ended/closed');
		else
			wndwidx = 1;
		end
	end
	for col = 1:scan_width
		%get all the pixels in the burst
		if(WINDOWMAP(wndwidx, col, 4)==1)  %equals foreground %a foreground pixel has been found
			east = col; %east will track the burst to the end of the burst
			burstpos = col;  %starting edge of the section
			label_index = label_index + 1;  %label the found foreground pixel(s)
			
			while ((east<scan_width)&&(WINDOWMAP(wndwidx, east+1, 4) == 1)) %find the east (right) edge of the section
				east = east + 1;
			end
			
			[leftmost, width, row_ends, moments] = label_window(burstpos, wndwidx, east, instream, HSV_RANGES);  %find all connected pixels for current object
			outobject = serialize_object(WINDOWMAP, leftmost, width, row_ends);  %put object data in serial format for streaming out
			Ellipse(label_index) = calculate_ellipse(moments);  %calculate bounding ellipse based on moments.  Note: moments are updated in labeling algorithm above
			Ellipse(label_index).y = (Ellipse(label_index).y+rowcount)-1;  %shift ellipse to location in image
%-----------------------------
% for displaying output don't need for final application 
%-----------------------------
% 			figure(base_fig_handle);
%------------------------------

			serialdata = deserialize_object(outobject); %create 2D object from serial data
	
			is_connected = check_connected(Ellipse(label_index));
			if is_connected
				Connected_grains = Connected_grains + 1;
%--------------------------------------------------
%need to recode separate_connected() for matlab coder...use temp for now
%--------------------------------------------------
                %separated_grains = separate_connected(serialdata);
                separated_grains = separate_connected_stub(serialdata);
				%--------------------------------------------------
				
%-----------------------------
% for displaying output don't need for final application 
%-----------------------------
% 				overlay_object(instream, separated_grains, rowcount, leftmost, [255 0 0]);
%               plot_ellipse(separated_grains, rowcount, leftmost);
% 			else
%               overlay_object(instream, serialdata, rowcount, leftmost, [0 0 255]);
%               draw_ellipse(Ellipse(label_index), 'LineWidth', 4);
%-----------------------------
			end
			
%-----------------------------
% for displaying output don't need for final application 
%-----------------------------
%			%draw_ellipse(Ellipse(label_index), 'LineWidth', 4);
%			video_frames(label_index) = getframe(gcf) ;
%-------------------------------

			process_object(outobject);		
		end
	end %for all colums in row
	
	WINDOWMAP(wndwidx,:,:) = []; %remove used rows
	[wdw_height, ~, ~] = size(WINDOWMAP);	
 	if((STREAMVALID == 0)&&(wdw_height<=1))
 		break;
	end
	rowcount = rowcount + 1;
	%STREAMVALID = update_flag(rowcount, instream);
end %while

%-----------------------------
% for displaying output don't need for final application 
%-----------------------------
% 	create_video(fname, video_frames);

%	str1 = sprintf('%s %d', 'Number of objects: ', label_index);
% 	str2 = sprintf('%s %d', 'Number of Connected objects: ', Connected_grains);
% 	str3 = sprintf('%s %d', 'Number of single objects: ', (label_index - Connected_grains));
% 	disp(str1);     disp(str2);     disp(str3);
%-----------------------------

end %main HOL function
%--------------------------------------------------------------------------
% base recursion algorithm
%--------------------------------------------------------------------------
function [left_position, width, obj_edges, outmoments] = label_window(sectionstart, row, col, instream, hsv_ranges)

%GLOBALS
global WINDOWMAP
global STREAMVALID

%PERSISTENT/STATIC
persistent object_edges %the position of the edges of the object for each row
persistent moments  %used to calculate the bounding ellipse
persistent mostleft %the position of the left-most pixel in the object.  Needed for offsetting the other pixels in the object

persistent recursion_cnt %keep track of # of recursions for initialization and final formatting

if isempty(recursion_cnt)	%initialize static/persistent variables at 1st recursion entry
	recursion_cnt = 1;
	object_edges = [sectionstart, col];
	mostleft = sectionstart;
	mostright = col;
	moments = struct('m00', 0, 'm10', 0, 'm01', 0, 'm11', 0, 'm02', 0, 'm20', 0);
else 
	recursion_cnt = recursion_cnt + 1; %
end

[height, ~] = size(object_edges); %current height of object (i.e. current number of rows)
if height<row  
	object_edges(row,:) = [col,col]; %add another row if 
end
if col>object_edges(row, 2) %if the column being checked is further than the right
	object_edges(row, 2) = col;
	mostright = col;
end

while ((col>1)&&(WINDOWMAP(row, col-1,4)==1)) %while p(x-1,y) == foreground
	col = col - 1; %track back to the beginning of the section
end %while
m=col; %x -> m  %m tracks the foreground pixels in each section while col stays fixed
if col<object_edges(row, 1)  %if the current foreground pixel is further left than the saved left
	object_edges(row, 1) = col;  
end;

%[num_rows, scan_width, ~] = size(instream);
[~, scan_width, ~] = size(WINDOWMAP);
%STREAMVALID = 1;

if ((m<=scan_width)&&(WINDOWMAP(row, m,4)==1)) %while pm,y == foreground 
	if (m<mostleft)
		mostleft = m; %check and update the most-left pixel in the object
	end
	
	datavalid = 1;	%"do-while" loop
	while (datavalid == 1)
		moments = update_moments(moments, row, m);  %update moments to calculate the bounding ellipse
		WINDOWMAP(row, m, 4) = 2; %label -> pm,y  %map the pixel as foreground
		
		if((m+1>scan_width)||(WINDOWMAP(row, m+1, 4)~=1)) %while pm,y == background
			datavalid = 0;
			if object_edges(row, 2) < m 
				object_edges(row, 2) = m; %check and update the right-most pixel in the object
			end
		end
		m = m + 1; %increase m for checking the next pixel in the row	
	end
end

%while ((col < m)&&(row<=num_rows)&&(col>0)&&(col<=scan_width)) %while (x<m)
while ((col < m)&&(col>0)&&(col<=scan_width)) %while (x<m)
	if ((row>1)&&(WINDOWMAP(row-1, col, 4)==1)) %if p(x,y) == foreground in previous row
		label_window(sectionstart, row-1, col, instream, hsv_ranges); %label_recursion(x, y) 
	end %if
	
	[wrows, ~, ~] = size(WINDOWMAP);	%Check if window size needs to be increased before checking next row
	if ((wrows == row)&&(STREAMVALID==1))
		%NOTE!!!  This gets a row of pixels when we have reached the edge of 
		% the window but we aren't finished finding connected pixels.  
		% get_scan() will return a full row of background-only pixels if  
		% the stream has finished (or end of file) when get_scan() is
		% called.  We do this (add an extra empty/background row at the end)
		% so that the algorithm can continue following around edge pixels.
		[WINDOWMAP(row+1,:, :), STREAMVALID] = get_scan(instream, hsv_ranges);
	end %if
	
	if (WINDOWMAP(row+1, col, 4) == 1) %if px,y+2 == foreground 
		label_window(sectionstart, row+1, col, instream, hsv_ranges); %label_recursion(x, y + 2)
	end %if
	col = col+1; %increase x;,
end %while

recursion_cnt = recursion_cnt - 1;
if recursion_cnt == 0  %recursion finished.  Finalize the output data.
	recursion_cnt = [];  %reset recursion count to empty for next object
	obj_edges = object_edges;
	left_position = mostleft;
	outmoments = moments;
	width = (mostright - mostleft)+1;
end

end %of label_recursion
%--------------------------------------------------------------------------
% get image data that has been saved to a text file.  This will help avoid
% warnings in Matlab coder App
%--------------------------------------------------------------------------
function IMG = imread_binary(binary_fname, num_rows, num_cols)
	fID = fopen(binary_fname);
	data = fread(fID);
	fclose(fID);
	IMG = uint8(reshape(data, num_rows, num_cols, 3));
	%imshow(IMG);
end

%--------------------------------------------------------------------------
% used to save image data to binary file.
%--------------------------------------------------------------------------
% function imwrite_binary(binary_fname, IMG)
% 	fID = fopen(binary_fname,'wb');
% 	fwrite(fID, IMG,'uint8');
% 	fclose(fID);
% end

%--------------------------------------------------------------------------
% Get 1D scan.  This uses an image as a source but final implementatoin
% will use a stream
%--------------------------------------------------------------------------
function [RGBApixels, streamvalid] = get_scan(img, hsv_ranges)
persistent imgidx
if isempty(imgidx)
	imgidx = 1;
end

[rows, cols, ~] = size(img);
if (rows<imgidx) 
	imgidx = rows;   % this is a workaround...it's a bug!!!!  fix me!!!
	RGBApixels = zeros(1,cols,4,'uint8'); %all other pixels have been initialzed to 0 above
	streamvalid = 0;
	imgidx = [];
else
	RGBApixels = zeros(1,cols,4,'uint8');
	for col=1:cols
		HSV = RGB2HSV(img(imgidx,col,1), img(imgidx,col,2), img(imgidx,col,3));
		bg = calcMask(HSV, hsv_ranges);
		if bg==0
			RGBApixels(1, col,1:3) = img(imgidx,col,:); %all other pixels have been initialzed to 0 above
			RGBApixels(1, col,4) = 1;
		end
	end
	streamvalid = 1;
	imgidx = imgidx + 1;
end %if-else

end %function
%--------------------------------------------------------------------------
% check if connected
%--------------------------------------------------------------------------
function [is_connected, ave_value] = check_connected(Ellipse, base_size)
%CONSTANT VARIABLES
MIN_AVE_COUNT = 5;

%PERSISTENT VARIABLES
persistent ave_cnt;
persistent CMA_ave;

if isempty(CMA_ave)
	ave_cnt = 0;
	CMA_ave = 0;
end
ellipse_size = Ellipse(end).l;

if (nargin~=2) %if the base size hasn't been set
	if (ave_cnt < MIN_AVE_COUNT)
		is_connected = 0;
		CMA_ave = CMA_ave + ((ellipse_size - CMA_ave)/(ave_cnt+1)); %calculate cumulative average
		ave_cnt = ave_cnt+1;
	else
		if (ellipse_size >= CMA_ave*1.6)
			is_connected = 1;
			%don't add oversized objects to average size
		else
			CMA_ave = CMA_ave + ((ellipse_size - CMA_ave)/(ave_cnt+1));  %calculate cumulative average
			is_connected = 0;
			ave_cnt = ave_cnt+1;
		end
	end
	ave_value = CMA_ave;	
else
	if (ellipse_size >= base_size*1.6)
		is_connected = 1;
	else
		is_connected = 0;
	end
	ave_value = base_size;
end

end
%--------------------------------------------------------------------------
% separate/segment connected grains
%--------------------------------------------------------------------------
function separated_img = separate_connected_stub(twoDobject)
 separated_img = twoDobject;
end
%--------------------------------------------------------------------------
% Need to recode separate_connected() for Matlab coder APP
%--------------------------------------------------------------------------
% %--------------------------------------------------------------------------
% % separate/segment connected grains
% %--------------------------------------------------------------------------
% function separated_img = separate_connected(twoDobject)
% rgb_img = twoDobject;
% gray_img = rgb2gray(rgb_img);
% level = graythresh(gray_img);
% binary_img = im2bw(rgb_img, level);
% 
% SE  = strel('Disk',4,8);
% morphed = imdilate(binary_img, SE);
% 
% [centers, radii, metric] = imfindcircles(morphed, [18 40], ...
%     'Sensitivity', 0.9100, ...
%     'EdgeThreshold', 0.30, ...
%     'Method', 'TwoStage', ...
%     'ObjectPolarity', 'Bright');
% 
% [imageSizeY, imageSizeX] = size(morphed);
% [columnsInImage rowsInImage] = meshgrid(1:imageSizeX, 1:imageSizeY);
% 
% centers = sortrows(centers); j = 0; new_centers = [];
% size_of_centers = size(centers);
% for i = 1:size_of_centers(1)
%     if j > i
%         i = j;
%     end
%     for j = i+1:size_of_centers(1)
%         if (abs(mean(centers(i,1) - centers(j,1))) > 52 ...
%                 || abs(mean(centers(i,2) - centers(j,2))) > 52)
%             if i == 1
%                 new_centers = [new_centers; centers(i,:)];
%             end
%             new_centers = [new_centers; centers(j,:)];
%             break;
%         end
%     end
%     if j == length(centers)
%         break;
%     end
% end
% 
% mask = zeros(size(morphed)); size_of_centers = size(new_centers);
% for i = 1:size_of_centers(1)
%     centerX = new_centers(i,1);
%     centerY = new_centers(i,2);
%     radius = radii(i);
%     circlePixels = (rowsInImage - centerY).^2 ...
%         + (columnsInImage - centerX).^2 <= radius.^2;
%     % % circlePixels is a 2D "logical" array.
%     mask(circlePixels) = 1;
% end
% 
% if isempty(new_centers)
%     size_of_centers = size(centers);
%     for i = 1:size_of_centers(1)
%     centerX = centers(i,1);
%     centerY = centers(i,2);
%     radius = radii(i);
%     circlePixels = (rowsInImage - centerY).^2 ...
%         + (columnsInImage - centerX).^2 <= radius.^2;
%     % % circlePixels is a 2D "logical" array.
%     mask(circlePixels) = 1;
%     end
% end
% 
% [rows, columns] = find(mask == 0);
% separated_img = rgb_img;
% for i = 1:length(rows)
%     separated_img(rows(i), columns(i), 1) = 0;
%     separated_img(rows(i), columns(i), 2) = 0;
%     separated_img(rows(i), columns(i), 3) = 0;
% end
% end %separate connected
%--------------------------------------------------------------------------
% Helper functions to convert numbers to uint8/byte array 
%--------------------------------------------------------------------------
function [outdata] = uint2array(num, numbytes)
	switch numbytes
		case 1
			outcast = cast(num, 'uint8');
		case 2
			outcast = cast(num, 'uint16');
		case 4
			outcast = cast(num, 'uint32');
		case 8
			outcast = cast(num, 'uint64');
		otherwise
			error('numbytes not valid!  Exiting');
% 			disp('numbytes not valid!  Exiting');
% 			exit;
	end	
	outdata = zeros(1, numbytes, 'uint8');
	outdata = typecast(outcast,'uint8');
end
function [outdata] = array2uint(inarray)
	[~, arraysize] = size(inarray);
	switch arraysize
		case 1
			outdata = typecast(inarray, 'uint8');
		case 2
			outdata = typecast(inarray, 'uint16');
		case 4
			outdata = typecast(inarray, 'uint32');
		case 8
			outdata = typecast(inarray, 'uint64');
		otherwise
			error('Unknown unsigned integer size!  Exiting...');
% 			disp('Unknown unsigned integer size!  Exiting...');
% 			exit;
	end
end
%--------------------------------------------------------------------------
% Build an image based on HOL data format
%--------------------------------------------------------------------------
function outdata = serialize_object(inimage, left_position, width, row_ends)
	global OBJHDRSIZE 
	global ROWHDRSIZE
	global SECTIONHDRSIZE
	
	ID = uint32(0);
	dataptr = OBJHDRSIZE + ROWHDRSIZE + SECTIONHDRSIZE + 1; %start of data
	
	[num_rows, ~] = size(row_ends);
	outdata = zeros((num_rows*width)+(num_rows*20), 'uint8');
	outdata(1) = uint8(0); %initialize so that data will be uint8.  
	[num_rows, ~, ~] = size(row_ends); %number of rows in object
	
	hdr_id = uint2array(ID,4);
	outdata(1:4) = hdr_id;
	hdr_num_rows = uint2array(num_rows,2);
	outdata(5:6) = hdr_num_rows; %put number of rows into stream header
	hdr_width = uint2array(width,2);
	outdata(7:8) = hdr_width; %width between left-right extrema, used for creating space to hold regenerated image

	num_sections = 1; %the object must have at least 1 section, otherwise it wouldn't be an object
	section_size = 0; %this uses a do-while loop, therefore starts at 0
	
	rowhdrptr = OBJHDRSIZE+1; %pointer to each row's header (set to point to the 1st row's header)
	sectionhdrptr = OBJHDRSIZE+ROWHDRSIZE+1; %pointer to the header of the first section in the row
	
	for row = 1:num_rows
		instartcol=row_ends(row,1); %this starts at the row's first pixel in the incoming object
		shift = row_ends(row,1)-left_position; %used to shift the data to the far left to minimize output object size
		num_cols = 1+(row_ends(row, 2)-row_ends(row, 1)); %number of pixels between the edges in this row
		
		idx=instartcol;
		while idx<instartcol+num_cols
			sectionpos = cast((idx-instartcol)+shift+1, 'uint16'); %(left) position of the current section
			while ((idx<instartcol+num_cols)&&(inimage(row, idx, 4)~=0)) %while the mapped pixels are foreground pixel
				outdata(dataptr:dataptr+2) = uint8(inimage(row,idx,1:3)); %set section data equal to foreground pixels
				dataptr = dataptr + 3; %each pixel has 3 bytes
				section_size = section_size+1;  %count the pixels in this section
				idx = idx + 1;	
			end		
			outdata(sectionhdrptr:sectionhdrptr+1)=uint2array(sectionpos, 2);  %add the previous section's header data
			outdata(sectionhdrptr+2:sectionhdrptr+3)=uint2array(section_size, 2);
			while ((idx<instartcol+num_cols)&&(inimage(row, idx, 4)==0))  %skip background pixels until next foreground pixel found
				num_sections = num_sections + 1;
				section_size = 0; %reset section size for next section in row since there are still foreground pixels in the row.
				sectionhdrptr = dataptr; %set the section pointer to the last valid data location
				dataptr=sectionhdrptr+SECTIONHDRSIZE;  %make space for subsection header data		
				while (inimage(row, idx, 4)==0) %don't need to check column because last column in row must be fg pixel
					idx=idx+1; %keep skipping until foreground pixel found
				end		
			end
		end
		outdata(rowhdrptr:rowhdrptr+1)=uint2array(num_sections, 2);  %save the number of sections in the row to the row header 
		
		rowhdrptr=dataptr;  %next row starts at end of data
		sectionhdrptr=rowhdrptr + ROWHDRSIZE; %set offset for section header
		dataptr = sectionhdrptr + SECTIONHDRSIZE;  %set start point of next set of pixel data

		num_sections = 1; %reset the number of sections for the next row
		section_size = 0; %reset the section size of the next row to 0 
	end
end %function
%--------------------------------------------------------------------------
% Build an image based on HOL data format
%--------------------------------------------------------------------------
function [outimage, label] = deserialize_object(indata)
	global OBJHDRSIZE 
	global ROWHDRSIZE
	global SECTIONHDRSIZE
	
	%label = indata(1);
	label = array2uint(indata(1:4));
	num_rows = array2uint(indata(5:6));
	object_width = array2uint(indata(7:8));
	%dataindex = 4; %start of 1st section header
	dataindex = OBJHDRSIZE+1; %start of 1st section header
	
	outimage=zeros(num_rows,object_width,3, 'uint8'); %initialize image
	%outimage(num_rows, object_width, 1:3) = uint8(0);	%initialize image  
	for row = 1:num_rows
		num_sections = array2uint(indata(dataindex:dataindex+1)); %num of sections in row
		%dataindex = dataindex + 1;
		dataindex = dataindex + ROWHDRSIZE;
		for sectioncnt = 1:num_sections
			%colstart = indata(dataindex);  %start of section of foreground pixels
			colstart = array2uint(indata(dataindex:dataindex+1));  %start of section of foreground pixels
			%sectionsize = indata(dataindex+1); %number of pixels in section
			sectionsize = array2uint(indata(dataindex+2: dataindex+3)); %number of pixels in section
			%dataindex = dataindex + 2; %make space for header
			dataindex = dataindex + SECTIONHDRSIZE; %make space for section header
			for sectionidx = 0: sectionsize-1
				outimage(row, colstart+sectionidx, 1:3) = indata(dataindex:dataindex+2);
				dataindex = dataindex + 3;
			end	
		end
	end
end %deserialize_image()
%--------------------------------------------------------------------------
% stub for process_object().  C++ code would start a new thread when
% calling this
%--------------------------------------------------------------------------
function process_object(serialized_object_data)
	tempdata = serialized_object_data(1) + 1;
%	imgout = deserialize_object(serialized_object_data); %create 2D object from serial data
end
%--------------------------------------------------------------------------
% accumulate moment of foreground pixels
%--------------------------------------------------------------------------
function M = update_moments(M, row, col)
	% If you want to calculate the moment based on the grayscale/color level then
	% uncomment the commented part (and fix the code)
	M.m00 = M.m00+((col^0)*(row^0)); %*(double(Img(j(cnt), i(cnt))));
	M.m10 = M.m10+((col^1)*(row^0)); %*(double(Img(j(cnt), i(cnt)))); 
	M.m01 = M.m01+((col^0)*(row^1)); %*(double(Img(j(cnt), i(cnt)))); 
	M.m11 = M.m11+((col^1)*(row^1)); %*(double(Img(j(cnt), i(cnt)))); 
	M.m02 = M.m02+((col^0)*(row^2)); %*(double(Img(j(cnt), i(cnt)))); 
	M.m20 = M.m20+((col^2)*(row^0)); %*(double(Img(j(cnt), i(cnt)))); 
end %update moments
%--------------------------------------------------------------------------
% calculate the best-fit ellipse based on the foreground moments
%--------------------------------------------------------------------------
function E = calculate_ellipse(M)

E=struct('x', 0, 'y', 0, 'theta', 0, 'w', 0, 'l', 0, 'x1', 0, 'y1', 0, 'x2', 0, 'y2', 0);

% --- Ellipse properties
% Barycenter
E.x = M.m10/M.m00;
E.y = M.m01/M.m00;

% Central moments (intermediary step)
a = M.m20/M.m00 - ((E.x)^2);
b = 2*(M.m11/M.m00 - ((E.x)*(E.y)));
c = M.m02/M.m00 - ((E.y)^2);

% Orientation (radians)
E.theta = 1/2*atan(b/(a-c)) + (a<c)*pi/2;

% Minor and major axis
E.w = sqrt(8*(a+c-sqrt(b^2+(a-c)^2)))/2.0;
E.l = sqrt(8*(a+c+sqrt(b^2+(a-c)^2)))/2.0;

% Ellipse focal points
d = sqrt(E.l^2-E.w^2);
E.x1 = E.x + d*cos(E.theta);
E.y1 = E.y + d*sin(E.theta);
E.x2 = E.x - d*cos(E.theta);
E.y2 = E.y - d*sin(E.theta);

end

%--------------------------------------------------------------------------
% Matlab coder App has problems with the below and since it's not needed 
% in the target application, we can comment them out 
%--------------------------------------------------------------------------

%--------------------------------------------------------------------------
% update streamvalid flag by checking the instream/image
%--------------------------------------------------------------------------
% function streamvalid = update_flag(rowcount, instream)
% str = '';
% try
%     instream(rowcount, :, :);
% catch
%     str = 'No further rows are available';
% end
% 
% if isempty(str)
%     streamvalid = 1;
% else
%     streamvalid = 0;
% end
% end %update_flag()
%--------------------------------------------------------------------------

%--------------------------------------------------------------------------
% clear matlab variables but keep breakpoints
%--------------------------------------------------------------------------
% function clearNoBP(varargin)
%       % returns all breakpoints into 's'
%       s = dbstatus;
%       % records all input arguements (varargin) into 'options'
%       options = '';
%       for i = 1:numel(varargin)
%           options = [options,',''',varargin{i},''''];
%       end
%       % Execute built-in clear function with input options in specified caller workspace
%       evalin('caller',['builtin(''clear''',options,')']);
%       % resets the breakpoints
%       dbstop(s);
% end
%--------------------------------------------------------------------------
% Display functions don't need for final application
%--------------------------------------------------------------------------
% %--------------------------------------------------------------------------
% % Superimpose/overlay detected object pixels over original image
% %--------------------------------------------------------------------------
% function overlay_object(baseimage, objectdata, shiftrow, shiftcol, RGB_color)
% 	outval = nargin;
% 	if outval < 5
% 		use_object_color = 1;
% 	else
% 		use_object_color = 0;
% 	end;
% 	
% 	[brows, bcols, ~] = size(baseimage);
% 	[orows, ocols, ~] = size(objectdata);
% 	
% 	for orow=1:orows
% 		for ocol=1:ocols
% 			if(((orow+shiftrow)>brows+1)||((ocol+shiftcol)>bcols+1))
% 				%do nothing if part of ojbect is outside base image boundaries;
% 			else
% 				if use_object_color == 1
% 					pixel_color = objectdata(orow, ocol, 1:3);
% 				else
% 					pixel_color = RGB_color;
% 				end
% 				if objectdata(orow, ocol, 1)==[0 0 0];
% 					%skip background pixels
% 				else
% 					baseimage((orow+shiftrow)-1,(ocol+shiftcol)-1,1:3) = pixel_color;
% 				end
% 			end
% 		end
% 	end
% 	imshow(baseimage);
% end
% %--------------------------------------------------------------------------
% % create avi video based on image frames
% %--------------------------------------------------------------------------
% function create_video(filename, vid_frames)
% 	[~,name,~] = fileparts(filename);
% 	fname = strcat(name,'.avi');
% 	
% 	% create the video writer with 1 fps
% 	writerObj = VideoWriter(fname);
% 	
% 	% set the seconds per image
% 	writerObj.FrameRate = 2;
% 
% 	% open the video writer
% 	open(writerObj);
% 	
% 	% write the frames to the video
% 	for framecnt=1:length(vid_frames)
% 		
% 		% convert the image to a frame
% 		frame = vid_frames(framecnt);    
% 		writeVideo(writerObj, frame);
% 	end
% 	% close the writer object
% 	close(writerObj);
% end
% %--------------------------------------------------------------------------
% % plot ellipses onto image
% %--------------------------------------------------------------------------
% function plot_ellipse( separated_img, shiftrow, shiftcol)
%     rgb_img = separated_img;
%     gray_img = rgb2gray(rgb_img);
%     level = graythresh(gray_img);
%     binary_img = im2bw(rgb_img, level);
% 
%     SE  = strel('Disk',3,8);
%     morphed = imdilate(binary_img, SE);
%     hold on
% 
%     eR = regionprops(binary_img,{...
%         'Centroid',...
%         'MajorAxisLength',...
%         'MinorAxisLength',...
%         'Orientation'});
% 
%     t = linspace(0,2*pi,50);
%     for k = 1:length(eR)
%         a = eR(k).MajorAxisLength/2;
%         b = eR(k).MinorAxisLength/2;
%         Xc = eR(k).Centroid(1);
%         Yc = eR(k).Centroid(2);
%         phi = deg2rad(-eR(k).Orientation);
%         x = Xc + a*cos(t)*cos(phi) - b*sin(t)*sin(phi);
%         y = Yc + a*cos(t)*sin(phi) + b*sin(t)*cos(phi);
%         plot((x+shiftcol-1),(y+shiftrow-1),'b','Linewidth',4)
%     end
% end
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
