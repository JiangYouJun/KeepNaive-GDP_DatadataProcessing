#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
using namespace std;

int main() {
	cout.setf(ios_base::fixed);
	long double gdp[2][50] = {};
	int gdp_size = 0;
	string location;

	//读取读取文件位置
	ifstream location_read;
	location_read.open("source//location.txt");
	location_read >> location;
	location_read.close();

	//读取两组数据
	ifstream resource_read;
	resource_read.open(location);
	for (int i = 0; i < 50; i++) {
		resource_read >> gdp[0][i];
		if (gdp[0][i] < 100) {
			gdp[1][0] = gdp[0][i];
			gdp[0][i] = 0;
			gdp_size = i;
			break;

		}
	}
	for (int i = 1; i < 50; i++) {
		resource_read >> gdp[1][i];
	}
	resource_read.close();

	//对数据进行处理
	int best_num = 0;
	long double best_R = 0, best_R1 = 0, best_R2 = 0;
	long double best_b1 = 0, best_b01 = 0, best_b2 = 0, best_b02 = 0;
	long double _SSE1 = 0, _SSE2 = 0, _SSR1 = 0, _SSR2 = 0;

	ofstream file_test;
	file_test.setf(ios_base::fixed);
	file_test.open("output//debug.loggg");
	for (int i = 1; i < gdp_size - 1; i++) {

		//计算一元线性回归的拟合优度
		file_test << i << endl;
		long double sum_x1 = 0, sum_y1 = 0, sum_xy1 = 0, sum_xpow1 = 0, sum_ypow1 = 0;
		long double sum_x2 = 0, sum_y2 = 0, sum_xy2 = 0, sum_xpow2 = 0, sum_ypow2 = 0;

		//计算sum值
		for (int j = 0; j <= i; j++) {
			sum_x1 = sum_x1 + gdp[0][j];
			sum_y1 = sum_y1 + gdp[1][j];
			sum_xy1 = sum_xy1 + gdp[0][j] * gdp[1][j];
			sum_xpow1 = sum_xpow1 + gdp[0][j] * gdp[0][j];
			sum_ypow1 = sum_ypow1 + gdp[1][j] * gdp[1][j];
			file_test 
				<< "x1_" << j + 1 << " = " << gdp[0][j] << " - "
				<< "y1_" << j + 1 << " = " << gdp[1][j] << endl;

		}
		for (int j = i; j < gdp_size; j++) {
			sum_x2 = sum_x2 + gdp[0][j];
			sum_y2 = sum_y2 + gdp[1][j];
			sum_xy2 = sum_xy2 + gdp[0][j] * gdp[1][j];
			sum_xpow2 = sum_xpow2 + gdp[0][j] * gdp[0][j];
			sum_ypow2 = sum_ypow2 + gdp[1][j] * gdp[1][j];
			file_test << "x2_" << j + 1 << " = " << gdp[0][j] << " - "
				<< "y2_" << j + 1 << " = " << gdp[1][j] << endl;
		}

		file_test
			<< "sum_x1 = " << sum_x1 << endl
			<< "sum_y1 = " << sum_y1 << endl
			<< "sum_xy1 = " << sum_xy1 << endl
			<< "sum_xpow1 = " << sum_xpow1 << endl
			<< "sum_ypow1 = " << sum_ypow1 << endl
			<< "sum_x2 = " << sum_x2 << endl
			<< "sum_y2 = " << sum_y2 << endl
			<< "sum_xy2 = " << sum_xy2 << endl
			<< "sum_xpow2 = " << sum_xpow2 << endl
			<< "sum_ypow2 = " << sum_ypow2 << endl;

		//计算一阶线性拟合系数
		int i2 = gdp_size - i;
		long double b1 = ((i + 1) * sum_xy1 - sum_x1 * sum_y1) / ((i + 1) * sum_xpow1 - sum_x1 * sum_x1);
		long double b01 = (sum_xpow1 * sum_y1 - sum_x1 * sum_xy1) / ((i + 1) * sum_xpow1 - sum_x1 * sum_x1);
		long double b2 = (i2 * sum_xy2 - sum_x2 * sum_y2) / (i2 * sum_xpow2 - sum_x2 * sum_x2);
		long double b02 = (sum_xpow2 * sum_y2 - sum_x2 * sum_xy2) / (i2 * sum_xpow2 - sum_x2 * sum_x2);

		file_test 
			<< "b1 = " << b1 << endl
			<< "b01 = " << b01 << endl
			<< "b2 = " << b2 << endl
			<< "b02 = " << b02 << endl;

		//计算SSR和SSE
		long double ave_x1 = sum_x1 / (i + 1);
		long double ave_y1 = sum_y1 / (i + 1);
		long double ave_x2 = sum_x2 / i2;
		long double ave_y2 = sum_y2 / i2;

		file_test
			<< "ave_x1 = " << ave_x1 << endl
			<< "ave_y1 = " << ave_y1 << endl
			<< "ave_x2 = " << ave_x2 << endl
			<< "ave_y2 = " << ave_y2 << endl;

		long double sum_SSR1 = 0, sum_SSE1 = 0;
		long double sum_SSR2 = 0, sum_SSE2 = 0;
		long double r1 = 0, r2 = 0, r = 0;

		for (int j = 0; j <= i; j++) {
			sum_SSR1 = sum_SSR1 + pow((b01 + b1 * gdp[0][j] - b01 - b1 * ave_x1), 2);
			sum_SSE1 = sum_SSE1 + pow((gdp[1][j] - b01 - b1 * ave_x1), 2);
		}
		for (int j = i; j < gdp_size; j++) {
			sum_SSR2 = sum_SSR2 + pow((b02 + b2 * gdp[0][j] - b02 - b2 * ave_x2), 2);
			sum_SSE2 = sum_SSE2 + pow((gdp[1][j] - b02 - b2 * ave_x2), 2);
		}
		r1 = sum_SSR1 / sum_SSE1;
		r2 = sum_SSR2 / sum_SSE2;
		r = r1 * r2;
		
		file_test
			<< "sum_SSR1 = " << sum_SSR1 << endl
			<< "sum_SSE1 = " << sum_SSE1 << endl
			<< "sum_SSR2 = " << sum_SSR2 << endl
			<< "sum_SSE2 = " << sum_SSE2 << endl
			<< "r1 = " << r1 << endl
			<< "r2 = " << r2 << endl
			<< "r = " << r << endl;

		//选取最好的R，并记录i值
		if (best_R < r) {
			best_R = r;
			best_R1 = r1;
			best_R2 = r2;
			best_num = i + 1;
			best_b01 = b01;
			best_b02 = b02;
			best_b1 = b1;
			best_b2 = b2;
			_SSE1 = sum_SSE1;
			_SSE2 = sum_SSE2;
			_SSR1 = sum_SSR1;
			_SSR2 = sum_SSR2;
		}

		file_test << endl;
	}
	file_test.close();

	//将算出的最优分段函数输出至可读文本
	ofstream file_output;
	file_output.setf(ios_base::fixed);
	file_output.open("output//output.txt");
	for (int i = 0; i < gdp_size; i++) {
		file_output.precision(0);
		file_output << gdp[0][i];
		file_output << " - ";
		file_output.precision(1);
		file_output << gdp[1][i] << endl;
	}
	file_output << endl;
	file_output.precision(4);
	file_output
		<< "best_num = " << best_num << endl
		<< "best_R1 = " << best_R1 << endl
		<< "best_R2 = " << best_R2 << endl
		<< "best_R = " << best_R << endl;
	file_output.close();

	//将原函数输出至primitive.data
	ofstream primitive;
	primitive.setf(ios_base::fixed);
	primitive.open("output//primitive.data");
	for (int i = 0; i < gdp_size; i++) {
		primitive.precision(0);
		primitive << gdp[0][i] << '\t';
		primitive.precision(1);
		primitive << gdp[1][i] << endl;
	}
	primitive.close();

	//输出分段函数1至piecewise_functions1.data
	ofstream piecewise1;
	piecewise1.setf(ios_base::fixed);
	piecewise1.open("output//piecewise_functions1.data");
	for (int i = 0; i < best_num + 1; i++) {
		piecewise1.precision(0);
		piecewise1 << gdp[0][i] << '\t';
		piecewise1.precision(1);
		piecewise1 << gdp[1][i] << endl;
	}
	piecewise1.close();

	//输出分段函数2至piecewise_functions2.data
	ofstream piecewise2;
	piecewise2.setf(ios_base::fixed);
	piecewise2.open("output//piecewise_functions2.data");
	for (int i = best_num; i < gdp_size; i++) {
		piecewise2.precision(0);
		piecewise2 << gdp[0][i] << '\t';
		piecewise2.precision(1);
		piecewise2 << gdp[1][i] << endl;
	}
	piecewise2.close();

	//输出分段线性回归的参数及拟合优度至parameter1.data
	ofstream parameter1;
	parameter1.setf(ios_base::fixed);
	parameter1.precision(6);
	parameter1.open("output//parameter1.data");
	parameter1
		<< best_num << '\t'
		<< best_b1 << '\t'
		<< best_b01 << '\t'
		<< best_R1 << '\t'
		<< _SSE1 << '\t'
		<< _SSR1 << '\t'
		<< endl;
	parameter1.close();

	//输出分段线性回归的参数及拟合优度至parameter2.data
	ofstream parameter2;
	parameter2.setf(ios_base::fixed);
	parameter2.precision(6);
	parameter2.open("output//parameter2.data");
	parameter2
		<< best_num << '\t'
		<< best_b2 << '\t'
		<< best_b02 << '\t'
		<< best_R2 << '\t'
		<< _SSE2 << '\t'
		<< _SSR2 << '\t'
		<< endl;
	parameter2.close();


	cout << "done!" << endl;
	return 0;
}
