#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <gmp.h>
#include "data.h"

using namespace std; 

Data::Data() {
	database = new DataNode[10000];
	ifstream data_file;
	char buffer[50];
	data_file.open("census.csv", ios::in);

	int j = 0;
	while (!data_file.eof()) {
		data_file.getline(buffer, 50);

		string *re;
		re = split(buffer, ',');

		database[j].age = atoi((*(re + 0)).c_str());
		database[j].sex = *(re + 1);
		database[j].race = *(re + 2);
		database[j].UNION = *(re + 3);
		database[j].zipcode = *(re + 4);
		database[j].income = atoi((*(re + 5)).c_str());
		database[j].vote = *(re + 6);

		//show(j);
		j++;
	}
	//epsilon = 1;
}

void Data::show(int j) {
	cout << "database " << j << " age: " << database[j].age << endl;
	cout << "database " << j << " sex: " << database[j].sex << endl;
	cout << "database " << j << " race: " << database[j].race << endl;
	cout << "database " << j << " UNION: " << database[j].UNION << endl;
	cout << "database " << j << " zipcode: " << database[j].zipcode << endl;
	cout << "database " << j << " income: " << database[j].income << endl;
	cout << "database " << j << " vote: " << database[j].vote << endl;
	cout << endl;
}

double Data::over_40() {
	int i = 0;
	double sum = 0.0;
	while(i < 10000) {
		if(database[i].age > 40) {
			sum++;
		}
		i++;
	}
	double lambda = 1.0 / (*this).epsilon;
	sum += Lap(0, lambda);
	return sum;
}

double Data::average_income_male() {
	double ave = 0.0, count = 0.0;
	int i = 0;
	while(i < 10000) {
		if(database[i].sex == "M") {
			count++;
			ave = ave + database[i].income / 10000;
			//cout << ave << endl;
		}
		i++;
	}
	double lambda = 1.0 / (*this).epsilon;
	count += Lap(0, lambda);
	ave = ave * 10000 / count;
	ave += Lap(0, lambda);
	return ave;
}

double Data::average_income_female() {
	double ave = 0.0, count = 0.0;
	int i = 0;
	while (i < 10000) {
		if (database[i].sex == "F") {
			count++;
			ave = ave + database[i].income / 10000;
			//cout << ave << endl;
		}
		i++;
	}
	double lambda = 1.0 / (*this).epsilon;
	count += Lap(0, lambda);
	ave = ave * 10000 / count;
	ave += Lap(0, lambda);
	return ave;
}

double* Data::histogram() {
	double *result = new double [10];

	for (int i = 0; i < 10; i++) {
		//cout << *(hist + i) << endl;
		*(result + i) = 0.0;
	}

	int i = 0, j = 0;
	while(i < 10000) {
		result[database[i].age / 10]++;
		i++;
	}
	double lambda = 1.0 / (*this).epsilon;
	while(j < 10) {
		result[j] += Lap(0, lambda);
		j++;
	}
	return result;
}

string Data::vote_result() {
	int count[10] = {0};
	int i = 0;
	while(i < 10000) {
		if(database[i].vote == "0")
			count[0]++;
		else if(database[i].vote == "1")
			count[1]++;
		else if(database[i].vote == "2")
			count[2]++;
		else if(database[i].vote == "3")
			count[3]++;
		else if(database[i].vote == "4")
			count[4]++;
		else if(database[i].vote == "5")
			count[5]++;
		else if(database[i].vote == "6")
			count[6]++;
		else if(database[i].vote == "7")
			count[7]++;
		else if(database[i].vote == "8")
			count[8]++;
		else
			count[9]++;
		i++;
	}
	double sensitivity = 1.0;
	for(int i = 0; i < 10; i++)
		cout << count[i] << endl;


	mpf_t pro_0, pro_1, pro_2, \
		pro_3, pro_4, pro_5, \
		pro_6, pro_7, pro_8, pro_9;
	mpf_t e;
	mpf_init_set_d(e, (double)exp(1));
	mpf_init(pro_0);
	mpf_init(pro_1);
	mpf_init(pro_2);
	mpf_init(pro_3);
	mpf_init(pro_4);
	mpf_init(pro_5);
	mpf_init(pro_6);
	mpf_init(pro_7);
	mpf_init(pro_8);
	mpf_init(pro_9);
	mpf_pow_ui(pro_0, e, (*this).epsilon * count[0] / 2.0);
	mpf_pow_ui(pro_1, e, (*this).epsilon * count[1] / 2.0);
	mpf_pow_ui(pro_2, e, (*this).epsilon * count[2] / 2.0);
	mpf_pow_ui(pro_3, e, (*this).epsilon * count[3] / 2.0);
	mpf_pow_ui(pro_4, e, (*this).epsilon * count[4] / 2.0);
	mpf_pow_ui(pro_5, e, (*this).epsilon * count[5] / 2.0);
	mpf_pow_ui(pro_6, e, (*this).epsilon * count[6] / 2.0);
	mpf_pow_ui(pro_7, e, (*this).epsilon * count[7] / 2.0);
	mpf_pow_ui(pro_8, e, (*this).epsilon * count[8] / 2.0);
	mpf_pow_ui(pro_9, e, (*this).epsilon * count[9] / 2.0);
	
	mpf_t p_0, p_1, p_2, \
		p_3, p_4, p_5, \
		p_6, p_7, p_8, p_9;
	mpf_t sum;
	mpf_init(p_0);
	mpf_init(p_1);
	mpf_init(p_2);
	mpf_init(p_3);
	mpf_init(p_4);
	mpf_init(p_5);
	mpf_init(p_6);
	mpf_init(p_7);
	mpf_init(p_8);
	mpf_init(p_9);
	mpf_init(sum);
	mpf_set_d(sum, 0.0);
	mpf_add(sum, pro_0, pro_1);
	mpf_add(sum, sum, pro_2);
	mpf_add(sum, sum, pro_3);
	mpf_add(sum, sum, pro_4);
	mpf_add(sum, sum, pro_5);
	mpf_add(sum, sum, pro_6);
	mpf_add(sum, sum, pro_7); 
	mpf_add(sum, sum, pro_8);
	mpf_add(sum, sum, pro_9);

	mpf_div(p_0, pro_0, sum);
	mpf_div(p_1, pro_1, sum);
	mpf_div(p_2, pro_2, sum);
	mpf_div(p_3, pro_3, sum);
	mpf_div(p_4, pro_4, sum);
	mpf_div(p_5, pro_5, sum);
	mpf_div(p_6, pro_6, sum);
	mpf_div(p_7, pro_7, sum);
	mpf_div(p_8, pro_8, sum);
	mpf_div(p_9, pro_9, sum);

	gmp_printf("p_0: %.Ff\n", p_0);
	gmp_printf("p_1: %.Ff\n", p_1);
	gmp_printf("p_2: %.Ff\n", p_2);
	gmp_printf("p_3: %.Ff\n", p_3);
	gmp_printf("p_4: %.Ff\n", p_4);
	gmp_printf("p_5: %.Ff\n", p_5);
	gmp_printf("p_6: %.Ff\n", p_6);
	gmp_printf("p_7: %.Ff\n", p_7);
	gmp_printf("p_8: %.Ff\n", p_8);
	gmp_printf("p_9: %.Ff\n", p_9);


	double j = (double)rand() / RAND_MAX;
	mpf_t temp, temp2;
	mpf_init_set_d(temp, 0.0);
	mpf_init_set_d(temp2, 0.0);

	if(mpf_cmp_d(p_0, j) == 1)
		return "0";

	mpf_add(temp, p_0, p_1);
	if(mpf_cmp_d(p_0, j) == -1 && mpf_cmp_d(temp, j) == 1)
		return "1";

	mpf_add(temp2, temp, p_2);
	if(mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "2";

	mpf_add(temp, temp, p_2);
	mpf_add(temp2, temp2, p_3);
	if(mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "3";

	mpf_add(temp, temp, p_3);
	mpf_add(temp2, temp2, p_4);
	if (mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "4";

	mpf_add(temp, temp, p_4);
	mpf_add(temp2, temp2, p_5);
	if (mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "5";

	mpf_add(temp, temp, p_5);
	mpf_add(temp2, temp2, p_6);
	if (mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "6";

	mpf_add(temp, temp, p_6);
	mpf_add(temp2, temp2, p_7);
	if (mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "7";

	mpf_add(temp, temp, p_7);
	mpf_add(temp2, temp2, p_8);
	if (mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "8";

	mpf_add(temp, temp, p_8);
	mpf_add(temp2, temp2, p_9);
	if (mpf_cmp_d(temp, j) == -1 && mpf_cmp_d(temp2, j) == 1)
		return "9";
	return "Error";
}


void Data::SetEpsilon(double e) {
	(*this).epsilon = e;
}

//void Client::SetName(string name) {
//	(*this).name = name;
//}
//
//void Client::SetPasswd(string password) {
//	(*this).password = password;
//}
//
//void Client::SetIdentity(int id) {
//	(*this).identity = id;
//}

string* split(string str, char dem) {
	int index = 0;
	int i = 0;
	string *result = new string [8];
	//int end = str.find_last_of(dem);
	while (str.find_first_of(dem) != -1) {
		index = str.find_first_of(dem);
		if(str[0] == ' ') {
			int count = str.find_first_not_of(' ');
			str = str.substr(count, str.length());
			index -= count;
			result[i++] = str.substr(0, index);
		}else {
			result[i++] = str.substr(0, index);
		}
		str = str.substr(index + 1, str.length());
	}
	if(str[0] == ' ') {
		int count = str.find_first_not_of(' ');
		str = str.substr(count, str.length());
		result[i++] = str;
	}
	return result;
}

double Uniform() {
	//srand(time(NULL));
	return -0.5 + (double)rand() / (RAND_MAX + 1.0);
}

int sgn(double x) {
	return x < 0 ? -1 : 1;
}

double Lap(double miu, double lambda) {
	double U = Uniform();
	return miu - lambda * sgn(U) * log(fabs(1.0 - 2 * fabs(U)));
}


int main() {
	srand((unsigned)time(NULL));
	//srand(0);
	/*for(int i = 0; i < 10000; i++) {
		rand();
	}*/
	Data data;
	Client guest;

	while(true) {
		cout << "1. Login" << "\t" << "2. Sign up" << endl;
		string regist;
		
		cin >> regist;

		if(regist != "1" && regist != "2")
			continue;
		fstream client_list;
		client_list.open("guest.txt", ios::in | ios::out | ios::app);
		if(regist == "1") {
			//guest = new Client;
			string name, password, temp = "", temp_name = "";
			char buffer[50];
			int is_break = 0;
			cout << "Input your name:" << endl;
			cin >> name;
			while(!client_list.eof()) {
				client_list.getline(buffer, 50);
				temp = buffer;
				temp_name = temp.substr(0, temp.find_first_of(" "));

				if(name == temp_name) {
					is_break = 1;
					temp = temp.substr(temp.find_first_of(" ") + 1, temp.length());
					break;
				}
			}
			if(is_break == 0) {
				continue;
			}
			guest.name = name;

			cout << "Input your password:" << endl;
			cin >> password;
			string temp_pass = temp.substr(0, temp.find_first_of(" "));
			if(password == temp_pass) {
				guest.password = password;
				temp = temp.substr(temp.find_first_of(" ") + 1, temp.length());
			}else {
				continue;
			}
			
			guest.identity = atoi(temp.c_str());
			cout << "Login successful" << endl;
			break;
		}
		if(regist == "2") {
			string name, password, id;
			int is_continue = 0;
			cout << "Input your name:" << endl;
			cin >> name;
			while(!client_list.eof()) {
				char buffer[50] = {'\0'};
				string temp;
				client_list.getline(buffer, 50);
				temp = buffer;
				if(temp.find(name) == 0) {
					cout << "Name is existed, please change another name"<< endl;
					is_continue = 1;
					break;
				}
			}
			if(is_continue == 1)
				continue;
			client_list.close();
			client_list.open("guest.txt", ios::in | ios::out | ios::app);
			
			client_list.write("\n", 1);
			client_list.write(name.c_str(), name.length());
			client_list.write(" ", 1);
			cout << "Input your password:" << endl;
			cin >> password;
			client_list.write(password.c_str(), password.length());
			client_list.write(" ", 1);
			cout << "Input your id:" << endl;
			cin >> id;
			client_list.write(id.c_str(), id.length());
			cout << "Registed successful, please login" << endl;
		}

	}
	if(guest.identity == 1) {
		data.SetEpsilon(5);
	}
	if(guest.identity == 2) {
		data.SetEpsilon(0.1);
	}
	if(guest.identity == 3) {
		data.SetEpsilon(0.01);
	}

	double over_40_re, ave_income;
	over_40_re = data.over_40();
	ave_income = data.average_income_male();
	string result = data.vote_result();

	double *hist;
	hist = data.histogram();
	for(int i = 1; i <= 10; i++) {
		printf("From %d to %d: %.4f\n", i * 10 - 10, i * 10, *(hist + i - 1));
	}
	//cout << over_40_re << endl;
	printf("The number of people above 40s: %.4f\n", over_40_re);
	printf("Average income of male: %.4f\n", ave_income);
	printf("Average income of female: %.4f\n", data.average_income_female());
	cout << "选举结果:" << result << endl;

	return 0;
}