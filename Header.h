#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <bitset>

using namespace std;

vector<string> mnems{ "mov", "add" };
vector<string> all_regs{ "al","cl","dl","bl","ah","ch","dh","bh",
						 "ax","cx","dx","bx","sp","bp","si","di" };
// ����� ���������
bool r_f[2], acc_f[2], seg_f[2], bp_f[2], si_f[2],
bas_f[2], ind_f[2], imm_f;
int width[2];
const int N = 12;
vector<vector<string>::iterator> regs;
int constanta;

enum possible_ops
{
	r2rm,
	rm2r,
	imm2rm,
	imm2r,
	imm2a,
	mem2a,
	a2mem,
	rm2seg,
	seg2rm
};

struct el_mot
{
	string mn;
	enum possible_ops operation;
	short int osn_k;
};

struct el_mot shaboln[N] =
{
	/*r2rm  */  {"mov", r2rm, 0x44},	// �� ������������
	/*rm2r  */  {"mov", rm2r, 0x45},
	/*imm2rm*/  {"mov", imm2rm, 0xC6},	// �� ������������
	/*imm2r */  {"mov", imm2r, 0xB0},	// �� ������������
	/*mem2a */  {"mov", mem2a, 0x50},
	/*a2mem */  {"mov", a2mem, 0xA2},	// �� ������������
	/*rm2seg*/  {"mov", rm2seg, 0x8E},	// �� ������������
	/*seg2rm*/  {"mov", seg2rm, 0x8C},	// �� ������������

	/*rm2r  */  {"add", rm2r, 0x01},
	/*imm2rm*/  {"add", imm2rm, 0x41},	// �� ������������
	/*imm2a */  {"add", imm2a, 0x02}
};
