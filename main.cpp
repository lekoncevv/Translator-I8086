#include "Header.h"

auto find_mnem(const string &s)
{
	return find(mnems.begin(), mnems.end(), s);
}

auto find_reg(const string &s)
{
	return find(all_regs.begin(), all_regs.end(), s);
}

void kind_of_operand(int n, const string &op)
{
	// проверка типа операнда память/регистр/константу
	if (op.find('['))
	{
		try
		{
			constanta = stoi(op);
			imm_f = true;
			return;
		}
		catch (const std::exception&){	}
		// регистр
		r_f[n] = true;
		auto reg = find_reg(op);
		regs.push_back(reg);
		// проверка разрядности операнда
		width[n] = distance(all_regs.begin(), regs[n]) >> 3;
		// проверка на тип регистра
		if (regs[0][regs.size() - 1] == "ax" || regs[0][regs.size() - 1] == "al")
			acc_f[n] = true;
		// добавить проверку на сегменты
		//
	}
	else 
	{
		r_f[n] = false;
		int i = 1;
		while (op[i+1] != ']')
		{
			if (op[i] == '+')
				i++;
			char s[] = { op[i], op[i + 1], '\0' };
			if (!strcmp("bx", s) || !strcmp("bp", s))
			{
				bas_f[n] = true;
				if (!strcmp("bp", s))
					bp_f[n] = true;
			}
			else if (!strcmp("si", s) || !strcmp("di", s))
			{
				ind_f[n] = true;
				if (!strcmp("si", s))
					si_f[n] = true;
			}
			i++;
		}
	}
}

possible_ops calc_praddr(const string& mnem)
{
	// сравниваются флаги и на их основании вычисляется
	// какой шаблон команды используется
	if (mnem == mnems[0])
	{
		if (acc_f[0] && !r_f[1])
			return mem2a;
		if (acc_f[1] && r_f[0])
			return a2mem;
		if (imm_f && r_f[0])
			return imm2r;
		if (imm_f && !r_f[0])
			return imm2rm;
		if (seg_f[0])
			return rm2seg;
		if (seg_f[1])
			return seg2rm;
		if (r_f[1] && !r_f[0])
			return r2rm;
		if (r_f[0])
			return rm2r;
	}
	else if (mnem == mnems[1])
	{
		if (imm_f && acc_f[0])
			return imm2a;
		if (r_f[0])
			return rm2r;
	}
}

int find_shablon(const string &mnem, possible_ops pr_addr)
{
	for (int i = 0; i < N; i++)
		if (mnem == shaboln[i].mn && pr_addr == shaboln[i].operation)
			return i;
	return -1; 
}

int find_rm(int n)
{
	if (bas_f[n] && !bp_f[n] && ind_f[n] && si_f[n])
		return 0; // (BX) + (SI) + DISP,
	if (bas_f[n] && !bp_f[n] &&	ind_f[n] && !si_f[n])
		return 1; // (BX) + (DI) + DISP,
	if (bas_f[n] && bp_f[n] && ind_f[n] && si_f[n]) 
		return 2; // (BP) + (SI) + DISP,
	if (bas_f[n] && bp_f[n] && ind_f[n] && !si_f[n])
		return 3; // (BP) + (DI) + DISP,
	if (!bas_f[n] && ind_f[n] && si_f[n])
		return 4; // (SI) + DISP,
	if (!bas_f[n] && ind_f[n] && !si_f[n])
		return 5; // (DI) + DISP,
	if (bas_f[n] && bp_f[n] && !ind_f[n])
		return 6; // (BP)        + DISP,
	if (bas_f[n] && !bp_f[n] &&	!ind_f[n])
		return 7; // (BX)        + DISP,
	return -1;
}

void create_command(int shabl_num)
{
	int i = 0;
	// вывод основания
	cout << bitset<7>(shaboln[shabl_num].osn_k);
	// вывод разрядности
	cout << "-" << max(width[i], width[i + 1]) << "  ";
	switch (shabl_num)
	{
	case 0:;
	case 1:
		// если один из операндов память, то выводим нули, т.к косвенная адресация
		cout << min(r_f[i], r_f[i+1]) << "-" << min(r_f[i], r_f[i+1]);
		cout << "-" << bitset<3>(distance(all_regs.begin(), regs[i]) % 8);
		if (!r_f[i + 1])
			cout << "-" << bitset<3>(find_rm(i + 1)) << endl;
		else
			cout << "-" << bitset<3>(distance(all_regs.begin(), regs[i + 1]) % 8) << endl;
		break;
	case 2:;
	case 3:;
	case 4:
		cout << "0-0";
		cout << "-" << bitset<3>(distance(all_regs.begin(), regs[i]) % 8);
		if (!r_f[i + 1])
			cout << "-" << bitset<3>(find_rm(i + 1)) << endl;
		else
			cout << "-" << bitset<3>(distance(all_regs.begin(), regs[i + 1]) % 8) << endl;
		break;
	case 5:;
	case 6:;
	case 7:;
	case 8:
		// если один из операндов память, то выводим нули, т.к косвенная адресация
		cout << min(r_f[i], r_f[i + 1]) << "-" << min(r_f[i], r_f[i + 1]);
		cout << "-" << bitset<3>(distance(all_regs.begin(), regs[i]) % 8);
		if (!r_f[i + 1])
			cout << "-" << bitset<3>(find_rm(i + 1)) << endl;
		else
			cout << "-" << bitset<3>(distance(all_regs.begin(), regs[i + 1]) % 8) << endl;
		break;
	case 9:;
	case 10:
		if (!width[i])
			cout << bitset<8>(constanta) << endl;
		else
			cout << bitset<16>(constanta) << endl;
		break;

	default:
		break;
	}

}
void clear_flags()
{
	for(int i = 0; i < 2; i++)
	{
		r_f[i] = false; 
		acc_f[i] = false;
		seg_f[i] = false;
		bp_f[i] = false; 
		si_f[i] = false;
		bas_f[i] = false;
		ind_f[i] = false;
		width[i] = 0;
	}
	imm_f = false;
}

int main()
{
	ifstream out("примеры команд.txt");

	if (out.is_open())
	{
		string command;
		string n, operand;
		int shabl_num;

		while (getline(out, command))
		{
			stringstream comm(command);
			vector<string> operands;

			comm >> n;
			auto mnem = find_mnem(n);
			while (getline(comm, operand, ','))
				operands.push_back(operand);
			operands[0].erase(0, 1);
			cout << n << " " << operands[0] << "," << operands[1] << "\t";

			kind_of_operand(0, operands[0]);
			kind_of_operand(1, operands[1]);

			auto pr_addr = calc_praddr(*mnem);
			shabl_num = find_shablon(*mnem, pr_addr);
			create_command(shabl_num);

			clear_flags();
			regs.clear();
		}
	}
	out.close();
}