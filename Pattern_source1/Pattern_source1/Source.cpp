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
int blackorwhite[400] = { 0 };  //blackorwhite bool�� �� false�� ����.
int blackorwhite_count = 0;
int xpt = 0;    //10*10���� �˻��ϱ� ���� x��������.
int ypt = 0;    //10*10���� �˻��ϱ� ���� y��������.

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
	printf("���� ��θ� �Է��� �ּ��� : ");

	while (1)
	{
		gets_s(FileName, sizeof(FileName));
		if (_access(FileName, 0) == 0)
		{

			printf("-----------------------------------------------------\n");
			printf("Ű���� �Է½�(� Ű��) ���α׷� ����\n");
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
						cvSet2D(src, y, x, cvScalar(0, 0, 0)); //����ȭ
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
			lx = lx - 1;    //�ܰ����� ���� �߶󳾶� 1�ȼ��� ������ �����=> Canny�� ���� ������ �ȳ����� ���� �̸� ����.
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


			for (int x = 0; x < size.width; x++)  //outimg�� src ����.
			{

				for (int y = 0; y < size.height; y++)
				{

					xr = (float)x / ((float)size.width - 1) * xsize;
					yr = (float)y / ((float)size.height - 1) * ysize;
					CvScalar s = cvGet2D(src, ly + yr, lx + xr);

					cvSet2D(outimg, y, x, s);

				}
			}


			for (int x = 0; x < size.width; x++) //src�̹������� Ư�� ���� ���� �ȼ� ����
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
					cvSet2D(resize, y, x, c);     //�������� �̹����� 200*200ũ��� �� ����.

				}
				count_height = 0;

			}

			outimg = doCanny(resize, 0, 200, 5);  //cvCanny�� resize�� �����Ͽ� outimg�� ����.

			for (int y = 0; y < size.height; y++)     //Canny(�����Լ��� ����)�Լ��� �̿��� outimg�� �������� ���پ� �˻�. �̹����� ������ �ؾ߸� ���� ���� �񱳰���.
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

			/*�¿��Ī ���� �ҽ��ڵ�(�ʿ����)

			for (int y = 0; y<size.height; y++)     //�������� ���پ� �������鼭 resize �̹����� �¿��Ī�Ͽ� symimg�� ����.
			{

			for (int x = 0; x < size.width; x++)
			{
			CvScalar c = cvGet2D(resize, y, x);

			cvSet2D(symimg, y, size.width-x-1, c);
			}
			}
			for (int y = 0; y<size.height/2; y++)     //�������� ���پ� ������ ���ݱ��� �������鼭 resize�� '�¿�'��Ī �̹��� symimg�� 1��1 �ȼ� ��.
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


			for (int y = size.height/2; y<size.height ; y++)     //�������� ���پ� ���ݿ��� �Ʒ����� �������鼭 resize�� '�¿�'��Ī �̹��� symimg�� 1��1 �ȼ� ��.
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

				for (int y = ypt; y < ypt + 10; y++)   //blackorwhite int���� y��,x������ ���� ��ܺ��� ���簢������ �˻��Ͽ� ���� �̻��� �������� ���, bool ������ true����
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
				if (i == 1)    //i=0�϶� ������ ypt ���ڸ��� �����ֱ�.
				{
					ypt = 0;

				}
				else if (i % 20 == 0)
				{
					ypt = ypt + 10;
					xpt = 0;
				}
			}

			//   �����κ�------------------------------------------------�����ʿ�

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



			for (int i = 0; i < 400; i++)  //�迭ȭ ��Ų resize �ð�ȭ.
			{
				if (i == 0)
				{

				}
				else if (i % 20 == 0)
					printf("\n");
				if (blackorwhite[i + 1] == 1)                  //�迭 �������  **�������� blackorwhite[1]���� [400]���� ����ϴ� �� �����Ұ�. ù��°ĭ�� ������. �׷��� ĭ�� ����.
				{
					printf("%4d", i);
					//printf("��");
				}
				else
					printf("%4d", 0);
				//printf("��");

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

			//printf("%d  %d  %d  %d \n %d  %d  \n", lx, ly, hx, hy, xr, yr);                   //�󺧸� ��ǥ��

			if (h_count > 190) {
				if (blackorwhite[282] == 1)
					printf("4�Դϴ�.\n\n");
				else
					printf("1�Դϴ�.\n\n");
			}
			else if (w_count >= 128)
			{
				if (blackorwhite[380] == 1 || blackorwhite[119] == 1)
					printf("2�Դϴ�\n\n");
				else if (blackorwhite[278] == 1)
					printf("5�Դϴ�\n\n");
				else
					printf("7�Դϴ�\n\n");
			}

			else
			{
				if (upper_trans_count == 2 && lower_trans_count == 2)
				{
					if (cencheck == 0)
						printf("8�Դϴ�\n\n");
					else
						printf("0�Դϴ�\n\n");
				}
				else if (right_threecheck == 1 && lower_trans_count == 1)
					printf("9�Դϴ�\n\n");
				else if (upper_trans_count == 1 && left_threecheck != 2)
					printf("6�Դϴ�\n\n");
				else
					printf("3�Դϴ�\n\n");

			}

			/*{
			if (blackorwhite[149] == 0 && blackorwhite[169] == 0 && blackorwhite[189] == 0 && blackorwhite[209] == 0 && blackorwhite[229] == 0 && blackorwhite[249] == 0)
			printf("0�Դϴ�\n\n");
			else if (blackorwhite[142] == 0 && blackorwhite[143] == 0 && blackorwhite[202] == 0 && blackorwhite[203] == 0 && blackorwhite[242] == 0 && blackorwhite[243] == 0)
			printf("3�Դϴ�\n\n");
			else if (blackorwhite[135] == 0 && blackorwhite[136] == 0 && blackorwhite[137] == 0 && blackorwhite[138] == 0)
			printf("6�Դϴ�\n\n");
			else
			printf("8�Դϴ�\n\n");
			}*/


			printf("�ش� ������ ���� �� ���� �ȼ� ���� %d�Դϴ�(�̹��� ������ �ϳ��� y���� �����ϴ� x �ȼ���) \n", w_count);  //������ cvCanny�� �̿��� �����Լ��� ��ģ outimg�� ���� �ȼ� Ȯ���� ��
			printf("�ش� ������ ���� �� ������ �ȼ� ���� %d�Դϴ�(�ϳ��� x���� �����ϴ� y �ȼ���)\n\n", h_count);  //�������� src�� ���� ���پ� �˻��Ͽ� Ư�� x���ο� �����ϴ� �ȼ��� �ִ밪�� ����.


			printf("resize�� �׷��� �� ���򼱿��� ���� %d �� �ٲ�ϴ�\n\n", upper_trans_count);
			printf("resize�� �׷��� �Ʒ� ���򼱿��� ���� %d �� �ٲ�ϴ�\n\n", lower_trans_count);

			//printf("�ش���� �߰� ���κ� �¿��Ī �ۼ�Ʈ�� %3.1f�Դϴ�\n\n", up_leftright_sympercent);
			//printf("�ش���� �߰� �Ʒ��κ� �¿��Ī �ۼ�Ʈ�� %3.1f�Դϴ�\n\n", down_leftright_sympercent);




			printf("!!! %d!!!! \n", num);
			printf("���� ������ Ƚ�� : %d", upper_trans_count);
			printf("���� ������ Ƚ�� : %d", lower_trans_count);
			printf("��� Ȯ�� : %d ", cencheck);
			printf("3 Ȯ�� : %d", left_threecheck);
			printf("3 Ȯ�� : %d", right_threecheck);

			int k = cvWaitKey();
			if (k == 0)
				exit(0);
		}
		else
			printf("������ �����ϴ�.\n��Ȯ�� ��ġ�� �Է��� �ּ��� : ");

	}


	return 0;
}