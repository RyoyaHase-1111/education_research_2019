//Heartbeat_Elect.c

/*���ԃf�[�^�̎擾*/
#include "time.h"

/*�����ݒ�*/
#include<stdio.h>
#include <windows.h> 

/*VS��fopen���g�����߂̂��܂��Ȃ�*/
#pragma warning(disable: 4996)

int estr_g;

void Heartbeat_Elect(wchar_t *port) {

	//�֐�------------------------------------------------------------
	/*�t�@�C���p*/
	char filename[50];
	clock_t starttime;
	FILE *fp_ECG;
	double interval = 0;

	/*�V���A���ʐM�p*/
	HANDLE h;
	int i = 0;
	char sBuf[1];
	char str[100];
	int baudRate = 500000;
	unsigned long nn;
	DCB dcb;


	//-----------------------------------------------------------------

	//�t�@�C�����쐬---------------------------------------------------
	sprintf(filename, "%s_ECG.csv", timepoints);
	fp_ECG = fopen(filename, "a");
	//-----------------------------------------------------------------

	//�V���A���ʐM------------------------------------------------------
	h = CreateFile(port,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (h == INVALID_HANDLE_VALUE) {
		printf("ECG Open Error\n");
	}

	GetCommState(h, &dcb); // �V���A���|�[�g�̏�Ԃ��擾
	dcb.BaudRate = baudRate;
	dcb.Parity = NOPARITY;
	dcb.fParity = FALSE;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(h, &dcb); // �V���A���|�[�g�̏�Ԃ�ݒ�
	//-----------------------------------------------------------------






		//�V���A���|�[�g�ɑ΂��ēǂݍ���-----------------------------------
	while (1) {

		if (get_elapsed(starttime_all) >= 10) {
			break;
		}

		ReadFile(h, sBuf, 1, &nn, 0); // �V���A���|�[�g�ɑ΂���ǂݍ���
		if (nn == 1) {
			if (sBuf[0] == 10 || sBuf[0] == 13) { // '\r'��'\n'����M����ƕ���������
				if (i != 0) {
					/*�`�揈��(main����)*/
					str[i] = '\0';
					i = 0;
				}
			}
			else {
				str[i] = sBuf[0];
				i++;
			}
		}
	}

	starttime = clock();

	//�V���A���|�[�g�ɑ΂��ēǂݍ���-----------------------------------
	while (1) {


			ReadFile(h, sBuf, 1, &nn, 0); // �V���A���|�[�g�ɑ΂���ǂݍ���

			if (nn == 1) {
				if (sBuf[0] == 10 || sBuf[0] == 13) { // '\r'��'\n'����M����ƕ���������
					if (i != 0) {
						/*�`�揈��(main����)*/
						str[i] = '\0';
						i = 0;

						estr_g = atoi(str);

						fprintf(fp_ECG, "%lf,%s\n",interval, str);
						interval = interval + 0.01;
					}
				}
				else {
					str[i] = sBuf[0];
					i++;
				}
			}

		if (stop) break;
	}
	fclose(fp_ECG);
}
