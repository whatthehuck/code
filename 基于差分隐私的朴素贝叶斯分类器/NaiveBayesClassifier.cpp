#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cmath>
#include <time.h>

using namespace std;

#define M 3
#define N 15
#define PI 3.1415926

struct Px1 {
	int x1;
	int y;
	double P_x1_y;
};

struct Px2 {
	int x2;
	int y;
	double P_x2_y;
};

Px1 px1[6];
Px2 px2[6];

double P_y1 = 0.0, P_y2 = 0.0;

double mean_height_1 = 0.0, variance_height_1 = 0.0;
double mean_weight_1 = 0.0, variance_weight_1 = 0.0;
double mean_foot_1 = 0.0, variance_foot_1 = 0.0;
double mean_height_2 = 0.0, variance_height_2 = 0.0;
double mean_weight_2 = 0.0, variance_weight_2 = 0.0;
double mean_foot_2 = 0.0, variance_foot_2 = 0.0;
double mean2 = 0.0, variance2 = 0.0;



int training_set[M][N] = {
	{1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3},
	{1, 2, 2, 1, 1, 1, 2, 2, 3, 3, 3, 2, 2, 3, 3},
	{-1, -1, 1, 1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1}
};


double continuous_training_set[4][8] {
	{6, 5.92, 5.58, 5.92, 5, 5.5, 5.42, 5.75},
	{180, 190, 170, 165, 100, 150, 130, 150},
	{12, 11, 12, 10, 6, 8, 7, 9},
	{1, 1, 1, 1, 2, 2, 2, 2}
};


double Uniform() {
	return -0.5 + (double)rand() / (RAND_MAX + 1);
}

int sgn(double x) {
	return x < 0 ? -1 : 1;
}

double Laplace(double miu, double lambda) {
	double U = Uniform();
	return miu - lambda * sgn(U) * log(fabs(1.0 - 2.0 * fabs(U)));
}

void CalP(double miu, double lambda) {
	double noise = 0.0;

	vector<int> v_x1, v_x2, v_y, temp;

	vector<int> unique_x1, unique_x2, unique_y;

	vector<int>::iterator it1, it2, it3;

	//double P_y1 = 0.0, P_y2 = 0.0;
	//double P_x1_y[6] = {0.0}, P_x2_y[6] = {0.0};

	for(int i = 0; i < N; i++) {
		v_x1.push_back(training_set[0][i]);
		v_x2.push_back(training_set[1][i]);
		v_y.push_back(training_set[2][i]);
	
	}
	temp = v_x1;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_x1 = temp;

	temp = v_x2;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_x2 = temp;

	temp = v_y;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_y = temp;

	for(int i = 0; i < unique_x2.size(); i++) {
		cout << unique_x2[i] << endl;
	}

	it1 = v_x1.begin();
	it2 = v_x2.begin();
	it3 = v_y.begin();

	P_y1 = (double)count(v_y.begin(), v_y.end(), 1) / N;
	P_y2 = (double)count(v_y.begin(), v_y.end(), -1) / N;

	int pos = 0;
	for(int i = 0; i < unique_y.size(); i++) {
		//noise = Laplace(miu, lambda);
		for(int j = 0; j < unique_x1.size(); j++) {
			px1[pos].x1 = unique_x1[j];
			px1[pos].y = unique_y[i];

			int count_x1 = 0;
			for(int k = 0; k < N; k++) {
				if(training_set[0][k] == px1[pos].x1 && training_set[2][k] == px1[pos].y)
					count_x1++;
			}
			while(true) {
				noise = Laplace(miu, lambda);
				count_x1 += noise;
				if(count_x1 < 0)
					continue;
				else
					break;
			}
			px1[pos].P_x1_y = (double)(count_x1 + 1) / count(v_y.begin(), v_y.end(), px1[pos].y);
			pos++;
		}
	
	}

	pos = 0;
	for (int i = 0; i < unique_y.size(); i++) {

		for (int j = 0; j < unique_x2.size(); j++) {
			px2[pos].x2 = unique_x2[j];
			px2[pos].y = unique_y[i];

			int count_x2 = 0;
			for (int k = 0; k < N; k++) {
				if (training_set[1][k] == px2[pos].x2 && training_set[2][k] == px2[pos].y)
					count_x2++;
			}
			while(true) {
				noise = Laplace(miu, lambda);
				count_x2 += noise;
				if(count_x2 < 0) 
					continue;
				else
					break;
			}

			px2[pos].P_x2_y = (double)(count_x2 + 1) / count(v_y.begin(), v_y.end(), px2[pos].y);
			pos++;
		}

	}
	for(int i = 0; i < 6; i++) {
		cout << px1[i].x1 << " " << px1[i].y << " " << px1[i].P_x1_y << endl;
	}
	cout << endl;
	for (int i = 0; i < 6; i++) {
		cout << px2[i].x2 << " " << px2[i].y << " " << px2[i].P_x2_y << endl;
	}
}

void CalP_continuous() {
	vector<double> height, weight, foot, y;
	vector<double> unique_height, unique_weight, unique_foot, unique_y;
	vector<double> temp;
	
	for(int i = 0; i < 8; i++) {
		height.push_back(continuous_training_set[0][i]);
		weight.push_back(continuous_training_set[1][i]);
		foot.push_back(continuous_training_set[2][i]);
		y.push_back(continuous_training_set[3][i]);
	}

	temp = height;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_height = temp;

	temp = weight;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_weight = temp;

	temp = foot;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_foot = temp;

	temp = y;
	sort(temp.begin(), temp.end());
	temp.erase(unique(temp.begin(), temp.end()), temp.end());
	unique_y = temp;

	//double mean_height_1 = 0.0, variance_height_1 = 0.0;
	//double mean_weight_1 = 0.0, variance_weight_1 = 0.0;
	//double mean_foot_1 = 0.0, variance_foot_1 = 0.0;
	//double mean_height_2 = 0.0, variance_height_2 = 0.0;
	//double mean_weight_2 = 0.0, variance_weight_2 = 0.0;
	//double mean_foot_2 = 0.0, variance_foot_2 = 0.0;
	//double mean2 = 0.0, variance2 = 0.0;

	int pos = 0;
	double sum_height_1 = 0.0, sum_weight_1 = 0.0, sum_foot_1 = 0.0;
	double sum_height_2 = 0.0, sum_weight_2 = 0.0, sum_foot_2 = 0.0;

	for(int i = 0; i < 8; i++) {
		if(y[i] == 1) {
			sum_height_1 += height[i];
			sum_weight_1 += weight[i];
			sum_foot_1 += foot[i];
		}
		if(y[i] == 2) {
			sum_height_2 += height[i];
			sum_weight_2 += weight[i];
			sum_foot_2 += foot[i];
		}
	}
	
	mean_height_1 = (double)sum_height_1 / count(y.begin(), y.end(), 1);
	mean_weight_1 = (double)sum_weight_1 / count(y.begin(), y.end(), 1);
	mean_foot_1 = (double)sum_foot_1 / count(y.begin(), y.end(), 1);
	mean_height_2 = (double)sum_height_2 / count(y.begin(), y.end(), 2);
	mean_weight_2 = (double)sum_weight_2 / count(y.begin(), y.end(), 2);
	mean_foot_2 = (double)sum_foot_2 / count(y.begin(), y.end(), 2);


	for(int i = 0; i < 8; i++) {
		if(y[i] == 1) {
			variance_height_1 += (height[i] - mean_height_1) * (height[i] - mean_height_1);
			variance_weight_1 += (weight[i] - mean_weight_1) * (weight[i] - mean_weight_1);
			variance_foot_1 += (foot[i] - mean_foot_1) * (foot[i] - mean_foot_1);
		}
		if(y[i] == 2) {
			variance_height_2 += (height[i] - mean_height_2) * (height[i] - mean_height_2);
			variance_weight_2 += (weight[i] - mean_weight_2) * (weight[i] - mean_weight_2);
			variance_foot_2 += (foot[i] - mean_foot_2) * (foot[i] - mean_foot_2);
		}
	}
	
	variance_height_1 /= count(y.begin(), y.end(), 1) - 1;
	variance_weight_1 /= count(y.begin(), y.end(), 1) - 1;
	variance_foot_1 /= count(y.begin(), y.end(), 1) - 1;
	variance_height_2 /= count(y.begin(), y.end(), 2) - 1;
	variance_weight_2 /= count(y.begin(), y.end(), 2) - 1;
	variance_foot_2 /= count(y.begin(), y.end(), 2) - 1;

	cout << variance_height_1 << " " << variance_weight_1 << " " << variance_foot_1 << " "\
		<< variance_height_2 << " " << variance_weight_2 << " " << variance_foot_2 << endl;
	
}



int main() {
	srand((unsigned)time(NULL));

	double miu = 0.0, lambda = 0.0, epsilon = 0.0;
	cin >> epsilon;

	lambda = 1 / epsilon;

	CalP_continuous();
	//CalP(miu, lambda);
	int x1, x2;
	double height, weight, foot;
	cin >> x1 >> x2;

	double result[2] = {0.0};
	for(int i = 0; i < 6; i++) {
		result[0] = P_y1;
		if(px1[i].x1 == x1 && px1[i].y == 1) {
			result[0] *= px1[i].P_x1_y;
			break;
		}
	}
	for(int i = 0; i < 6; i++) {
		if (px2[i].x2 == x2 && px2[i].y == 1)
			result[0] *= px2[i].P_x2_y;
	}

	for (int i = 0; i < 6; i++) {
		result[1] = P_y2;
		if(px1[i].x1 == x1 && px1[i].y == -1) {
			result[1] *= px1[i].P_x1_y;
			break;
		}
	}
	for (int i = 0; i < 6; i++) {
		if (px2[i].x2 == x2 && px2[i].y == -1)
			result[1] *= px2[i].P_x2_y;
	}

	cout << result[0] << " " << result[1] << endl;

	cin >> height >> weight >> foot;
	
	double gaussian_1 = 0.0;
	gaussian_1 = (double)1 / 2;
	gaussian_1 *= 1 / sqrt(2 * PI * variance_height_1)\
				* exp(-(height - mean_height_1) * (height - mean_height_1) / 2 / variance_height_1);
	
	gaussian_1 *= 1 / sqrt(2 * PI * variance_weight_1)\
		* exp(-(weight - mean_weight_1) * (weight - mean_weight_1) / 2 / variance_weight_1);
	
	gaussian_1 *= 1 / sqrt(2 * PI * variance_foot_1)\
		* exp(-(foot - mean_foot_1) * (foot - mean_foot_1) / 2 / variance_foot_1);

	cout << gaussian_1 << endl;

	double gaussian_2 = 0.0;
	gaussian_2 = (double)1 / 2;
	gaussian_2 *= 1 / sqrt(2 * PI * variance_height_2)\
		* exp(-(height - mean_height_2) * (height - mean_height_2) / 2 / variance_height_2);

	gaussian_2 *= 1 / sqrt(2 * PI * variance_weight_2)\
		* exp(-(weight - mean_weight_2) * (weight - mean_weight_2) / 2 / variance_weight_2);

	gaussian_2 *= 1 / sqrt(2 * PI * variance_foot_2)\
		* exp(-(foot - mean_foot_2) * (foot - mean_foot_2) / 2 / variance_foot_2);

	cout << gaussian_2 << endl;

	return 0;
}