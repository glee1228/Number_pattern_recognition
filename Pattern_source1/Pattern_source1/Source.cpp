#include <opencv2\opencv.hpp>
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<io.h>




IplImage * src;
IplImage * resize;
IplImage * dst2;
IplImage * outimg;
IplImage * symimg;
int blackorwhite[400] = { 0 };  //blackorwhite bool형 에 false로 선언.
int blackorwhite_count = 0;
int xpt = 0;    //10*10으로 검사하기 위한 x시작지점.
int ypt = 0;    //10*10으로 검사하기 위한 y시작지점.

IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture)
{
	IplImage* out = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);

	cvCanny(in, out, lowThresh, highThresh, aperture);
	return out;

}

int main()
{
	int num = 0;
	int count_height = 0;
	int count_width = 0;
	int w_count = 0;
	int h_count = 0;
	bool continue_dot = false;
	int upper_trans_count = 0;
	int lower_trans_count = 0;
	int left_threecheck = 0;
	int right_threecheck = 0;
	int cencheck = 0;
	//int up_leftright_sym_count = 0;
	//float up_leftright_sympercent = 0;
	//int down_leftright_sym_count = 0;
	//float down_leftright_sympercent = 0;

	bool c_check = true;
	char FileName[50];
	printf("파일 경로를 입력해 주세요 : ");

	while (1)
	{
		gets_s(FileName, sizeof(FileName));
		if (_access(FileName, 0) == 0)
		{

			printf("-----------------------------------------------------\n");
			printf("키보드 입력시(어떤 키라도) 프로그램 종료\n");
			printf("-----------------------------------------------------\n");
			src = cvLoadImage(FileName);

			CvSize size = cvGetSize(src);
			resize = cvCreateImage(size, 8, 3);
			dst2 = cvCreateImage(size, 8, 3);
			outimg = cvCreateImage(size, 8, 3);
			symimg = cvCreateImage(size, 8, 3);


			int lx = 255;
			int hx = 0;
			int ly = 255;
			int hy = 0;

			CvScalar c;

			for (int x = 0; x < size.width; x++)
			{
				for (int y = 0; y < size.height; y++)
				{
					CvScalar origin = cvGet2D(src, y, x);
					if (origin.val[0] < 200 && origin.val[1] < 200 && origin.val[2] < 200)
						cvSet2D(src, y, x, cvScalar(0, 0, 0)); //이진화
					else
						cvSet2D(src, y, x, cvScalar(255, 255, 255));

					origin = cvGet2D(src, y, x);
					if (origin.val[0] < 200 && origin.val[1] < 200 && origin.val[2] < 200)
					{
						if (x < lx)
							lx = x;
						else if (x > hx)
							hx = x;
						if (y < ly)
							ly = y;
						else if (y > hy)
							hy = y;
					}

				}
			}
			lx = lx - 1;    //외곽선을 위해 잘라낼때 1픽셀씩 여유를 줘야함=> Canny를 쓰면 엣지가 안나오는 것을 미리 방지.
			ly = ly - 1;
			hx = hx + 1;
			hy = hy + 1;
			CvPoint p = cvPoint(lx, ly);
			CvPoint w = cvPoint(hx, ly);
			CvPoint e = cvPoint(lx, hy);
			CvPoint r = cvPoint(hx, hy);


			int xsize = hx - lx;
			int ysize = hy - ly;
			int xr = 0;
			int yr = 0;


			for (int x = 0; x < size.width; x++)  //outimg로 src 복사.
			{

				for (int y = 0; y < size.height; y++)
				{

					xr = (float)x / ((float)size.width - 1) * xsize;
					yr = (float)y / ((float)size.height - 1) * ysize;
					CvScalar s = cvGet2D(src, ly + yr, lx + xr);

					cvSet2D(outimg, y, x, s);

				}
			}


			for (int x = 0; x < size.width; x++) //src이미지에서 특정 줄의 수직 픽셀 검출
			{

				for (int y = 0; y < size.height; y++)
				{

					xr = (float)x / ((float)size.width - 1) * xsize;
					yr = (float)y / ((float)size.height - 1) * ysize;
					CvScalar origin = cvGet2D(src, ly + yr, lx + xr);



					c.val[0] = origin.val[0];
					c.val[1] = origin.val[1];
					c.val[2] = origin.val[2];

					if (c.val[0] == 0 && c.val[1] == 0 && c.val[2] == 0)
					{
						count_height++;
						if (h_count < count_height)
							h_count = count_height;
					}
					cvSet2D(resize, y, x, c);     //리사이즈 이미지에 200*200크기로 재 배정.

				}
				count_height = 0;

			}

			outimg = doCanny(resize, 0, 200, 5);  //cvCanny를 resize에 적용하여 outimg에 저장.

			for (int y = 0; y < size.height; y++)     //Canny(엣지함수를 적용)함수를 이용한 outimg를 수평으로 한줄씩 검사. 이미지의 엣지를 해야만 수평선 길이 비교가능.
			{

				for (int x = 0; x < size.width; x++)
				{


					CvScalar c = cvGet2D(outimg, y, x);

					if (c.val[0] > 200 || c.val[1] > 200 || c.val[2] > 200)
					{

						c.val[0] = 255;
						c.val[1] = 255;
						c.val[2] = 0;
						count_width++;
						if (w_count < count_width)
							w_count = count_width;

					}
					cvSet2D(outimg, y, x, c);
				}
				count_width = 0;

			}

			/*좌우대칭 관련 소스코드(필요없음)

			for (int y = 0; y<size.height; y++)     //수평으로 한줄씩 내려가면서 resize 이미지를 좌우대칭하여 symimg에 저장.
			{

			for (int x = 0; x < size.width; x++)
			{
			CvScalar c = cvGet2D(resize, y, x);

			cvSet2D(symimg, y, size.width-x-1, c);
			}
			}
			for (int y = 0; y<size.height/2; y++)     //수평으로 한줄씩 위에서 절반까지 내려가면서 resize와 '좌우'대칭 이미지 symimg과 1대1 픽셀 비교.
			{

			for (int x = 0; x < size.width; x++)
			{
			CvScalar sym = cvGet2D(symimg, y, x);
			CvScalar c = cvGet2D(resize, y, x);

			if (sym.val[0] == c.val[0] && sym.val[1] == c.val[1] && sym.val[2] == c.val[2])
			{
			up_leftright_sym_count++;
			}
			}
			}
			up_leftright_sympercent = (float)up_leftright_sym_count / 20000.0 * 100.0;


			for (int y = size.height/2; y<size.height ; y++)     //수평으로 한줄씩 절반에서 아래까지 내려가면서 resize와 '좌우'대칭 이미지 symimg과 1대1 픽셀 비교.
			{

			for (int x = 0; x < size.width; x++)
			{
			CvScalar sym = cvGet2D(symimg, y, x);
			CvScalar c = cvGet2D(resize, y, x);

			if (sym.val[0] == c.val[0] && sym.val[1] == c.val[1] && sym.val[2] == c.val[2])
			{
			down_leftright_sym_count++;
			}
			}
			}
			down_leftright_sympercent = (float)down_leftright_sym_count / 20000.0 * 100.0;
			*/




			for (int i = 0; i < 400; i++) {

				for (int y = ypt; y < ypt + 10; y++)   //blackorwhite int형에 y축,x축으로 왼쪽 상단부터 정사각형으로 검사하여 절반 이상이 검은색일 경우, bool 형으로 true선언
				{
					for (int x = xpt; x < xpt + 10; x++)
					{
						CvScalar blackorwhite = cvGet2D(resize, y, x);
						if (blackorwhite.val[0] == 0 && blackorwhite.val[1] == 0 && blackorwhite.val[2] == 0)
							blackorwhite_count++;
						/*blackorwhite.val[0] = 0;
						blackorwhite.val[1] = 0;
						blackorwhite.val[2] = 255;
						cvSet2D(resize, y, x, blackorwhite);*/
					}

				}
				xpt = xpt + 10;
				if (blackorwhite_count > 40)
				{
					blackorwhite[i] = 1;
				}
				blackorwhite_count = 0;
				if (i == 1)    //i=0일때 움직인 ypt 제자리로 돌려주기.
				{
					ypt = 0;

				}
				else if (i % 20 == 0)
				{
					ypt = ypt + 10;
					xpt = 0;
				}
			}

			//   오류부분------------------------------------------------수정필요

			for (int y = 0; y < size.height; y++)
			{


				for (int x = 0; x < size.width; x++)

				{
					if (x != 0)
					{
						CvScalar g = cvGet2D(resize, y, x);
						CvScalar s = cvGet2D(resize, y, x - 1);

						if (y == 68)
						{
							if (s.val[0] == 0 && g.val[0] == 255)
								upper_trans_count++;
							
						}

						if (y == 135)
						{
							if (s.val[0] == 0 && g.val[0] == 255)
								lower_trans_count++;
						}

						if (x == 100 && y == 90 && g.val[0] == 255)
							cencheck = 1;
						
					}

					if (y != 0)
					{
						CvScalar g = cvGet2D(resize, y, x);
						CvScalar s = cvGet2D(resize, y - 1, x);

						if (x == 30)
						{
							if (s.val[0] == 0 && g.val[0] == 255)
								left_threecheck++;
						}

					}

					if (y != 0)
					{
						CvScalar g = cvGet2D(resize, y, x);
						CvScalar s = cvGet2D(resize, y - 1, x);

						if (x == 180)
						{
							if (s.val[0] == 0 && g.val[0] == 255)
								right_threecheck++;
						}

					}


				}

			}



			for (int i = 0; i < 400; i++)  //배열화 시킨 resize 시각화.
			{
				if (i == 0)
				{

				}
				else if (i % 20 == 0)
					printf("\n");
				if (blackorwhite[i + 1] == 1)                  //배열 출력조건  **유의할점 blackorwhite[1]부터 [400]까지 출력하는 점 유의할것. 첫번째칸은 버리기. 그래야 칸에 맞음.
				{
					printf("%4d", i);
					//printf("■");
				}
				else
					printf("%4d", 0);
				//printf("□");

			}
			printf("\n\n\n\n");


			cvLine(src, p, w, cvScalar(255, 0, 0), 1);
			cvLine(src, p, e, cvScalar(255, 0, 0), 1);
			cvLine(src, e, r, cvScalar(255, 0, 0), 1);
			cvLine(src, w, r, cvScalar(255, 0, 0), 1);

			cvShowImage("image", src);
			cvShowImage("resize image", resize);
			cvShowImage("Out Image", outimg);
			//("Symmetric Image", symimg);

			//printf("%d  %d  %d  %d \n %d  %d  \n", lx, ly, hx, hy, xr, yr);                   //라벨링 좌표값

			if (h_count > 190) {
				if (blackorwhite[282] == 1)
					printf("4입니다.\n\n");
				else
					printf("1입니다.\n\n");
			}
			else if (w_count >= 128)
			{
				if (blackorwhite[380] == 1 || blackorwhite[119] == 1)
					printf("2입니다\n\n");
				else if (blackorwhite[278] == 1)
					printf("5입니다\n\n");
				else
					printf("7입니다\n\n");
			}

			else
			{
				if (upper_trans_count == 2 && lower_trans_count == 2)
				{
					if (cencheck == 0)
						printf("8입니다\n\n");
					else
						printf("0입니다\n\n");
				}
				else if (right_threecheck == 1 && lower_trans_count == 1)
					printf("9입니다\n\n");
				else if (upper_trans_count == 1 && left_threecheck != 2)
					printf("6입니다\n\n");
				else
					printf("3입니다\n\n");

			}

			/*{
			if (blackorwhite[149] == 0 && blackorwhite[169] == 0 && blackorwhite[189] == 0 && blackorwhite[209] == 0 && blackorwhite[229] == 0 && blackorwhite[249] == 0)
			printf("0입니다\n\n");
			else if (blackorwhite[142] == 0 && blackorwhite[143] == 0 && blackorwhite[202] == 0 && blackorwhite[203] == 0 && blackorwhite[242] == 0 && blackorwhite[243] == 0)
			printf("3입니다\n\n");
			else if (blackorwhite[135] == 0 && blackorwhite[136] == 0 && blackorwhite[137] == 0 && blackorwhite[138] == 0)
			printf("6입니다\n\n");
			else
			printf("8입니다\n\n");
			}*/


			printf("해당 숫자의 가장 긴 수평선 픽셀 값은 %d입니다(이미지 엣지의 하나의 y열에 존재하는 x 픽셀값) \n", w_count);  //수평선은 cvCanny를 이용해 엣지함수를 거친 outimg를 토대로 픽셀 확인한 값
			printf("해당 숫자의 가장 긴 수직선 픽셀 값은 %d입니다(하나의 x열에 존재하는 y 픽셀값)\n\n", h_count);  //수직선은 src를 토대로 한줄씩 검사하여 특정 x라인에 존재하는 픽셀의 최대값을 저장.


			printf("resize에 그려진 위 수평선에서 색이 %d 번 바뀝니다\n\n", upper_trans_count);
			printf("resize에 그려진 아래 수평선에서 색이 %d 번 바뀝니다\n\n", lower_trans_count);

			//printf("해당숫자 중간 윗부분 좌우대칭 퍼센트는 %3.1f입니다\n\n", up_leftright_sympercent);
			//printf("해당숫자 중간 아랫부분 좌우대칭 퍼센트는 %3.1f입니다\n\n", down_leftright_sympercent);




			printf("!!! %d!!!! \n", num);
			printf("윗줄 지나간 횟수 : %d", upper_trans_count);
			printf("밑줄 지나간 횟수 : %d", lower_trans_count);
			printf("가운데 확인 : %d ", cencheck);
			printf("3 확인 : %d", left_threecheck);
			printf("3 확인 : %d", right_threecheck);

			int k = cvWaitKey();
			if (k == 0)
				exit(0);
		}
		else
			printf("파일이 없습니다.\n정확한 위치를 입력해 주세요 : ");

	}


	return 0;
}