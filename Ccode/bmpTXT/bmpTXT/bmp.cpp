
#define _CRT_SECURE_NO_WARNINGS  

#include <iostream>  
#include <windows.h>  
#include <stdlib.h>  
#include<math.h>  
#include <iomanip>   
#include <fstream>  
using namespace std;

unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��  
int bmpWidth;//ͼ��Ŀ�  
int bmpHeight;//ͼ��ĸ�  
RGBQUAD *pColorTable;//��ɫ��ָ��  
int biBitCount;//ͼ�����ͣ�ÿ����λ��  

//��ͼ���λͼ���ݡ����ߡ���ɫ��ÿ����λ�������ݽ��ڴ棬�������Ӧ��ȫ�ֱ�����  
bool readBmp(char *bmpName)
{
	FILE *fp = fopen(bmpName, "rb");//�����ƶ���ʽ��ָ����ͼ���ļ�  

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��  
	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //��ȡͼ����ߡ�ÿ������ռλ������Ϣ  

	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	biBitCount = head.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����  

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256  

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�  
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	fclose(fp);//�ر��ļ�  

	return 1;//��ȡ�ļ��ɹ�  
}


//����һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ,����д��ָ���ļ���  
bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{
	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������  
	if (!imgBuf)
		return 0;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0  
	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���  
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//�Զ�����д�ķ�ʽ���ļ�  
	FILE *fp = fopen(bmpName, "wb");

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ  
	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp����  

	//bfSize��ͼ���ļ�4����ɲ���֮��  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��  
	fileHead.bfOffBits = 54 + colorTablesize;

	//д�ļ�ͷ���ļ�  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ  
	BITMAPINFOHEADER head;

	head.biBitCount = biBitCount;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lineByte*height;
	head.biWidth = width;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;

	//дλͼ��Ϣͷ���ڴ�  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//дλͼ���ݽ��ļ�  
	fwrite(imgBuf, height*lineByte, 1, fp);

	//�ر��ļ�  
	fclose(fp);

	return 1;

}


//����Ϊ���صĶ�ȡ����  
void doIt()
{

	//����ָ��BMP�ļ����ڴ棬ע��Ҫ��"nv.BMP"�ļ��͡�Դ.cpp"�������ͬһ���ļ�����  
	char readPath[] = "test.bmp";
	readBmp(readPath);

	//ѭ��������ͼ�������  
	//ÿ���ֽ���  
	int lineByte = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;

	//ѭ����������Բ�ɫͼ�񣬱���ÿ���ص���������  
	int m = 0;

	
	ofstream outfile("out.txt", ios::in | ios::trunc);

	if (!outfile)
	{
		cout << "open error!" << endl;
		exit(1);
	}
	else if (biBitCount == 24)
	{   
		//��ɫͼ��  
		for (int i = bmpHeight; i>0; i -= 3)
		{
			for (int j = 0; j<bmpWidth; j += 3)
			{
				int x;
				for (int k = 0; k<3; k++)  
				{ 
					m = *(pBmpBuf + i*lineByte + j * 3 + k);
					if (k == 0)
					{
						x = 0;
					}
					x += m;
					if (k == 2 && x == 0)
					{
						outfile << "*";
					}
					if (k == 2 && x != 0)
					{
						outfile << " ";
					}
				}
			}
			outfile << endl;
		}
	}
	cout << "�ı����ɳɹ�������" << endl;
	//��ͼ�����ݴ���  
	//char writePath[] = "after.bmp";//ͼƬ������ٴ洢  
	//saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

	//��������� 
	delete[]pBmpBuf;

	if (biBitCount == 8)
		delete[]pColorTable;
}

void main()
{
	doIt();
}