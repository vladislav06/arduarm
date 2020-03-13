#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "MVector.h"
#include <bitset>
#include <climits>
#include"usb.h"
using namespace std;
//			команды 
#define move 199		// движение		
#define wait 202		//ждем

//------------------------
MVector::point dest;



struct angles
{
	float angle[2];
	float rootAngle;
};

struct pM
{
	MVector::point p[3];
};

struct setup {

	uint8_t speedCOM = 0b11001001;
	uint8_t speedDAT = 60;

};

setup settings;

struct data_E {

	angles angle;
	int wait_s;
};
	
//-----------------------

void out(MVector::point a) {
	std::cout << "++++++++++" << endl;
	std::cout << a.x << endl;
	std::cout << a.y << endl;
	std::cout << a.z << endl;
	std::cout << "++++++++++" << endl;
};



pM solver(MVector::point dest)
{
	MVector::point p[3] = {
		{0,0,0},//x,y,z
		{10,0,0},
		{20,0,0},
	};
	pM solved;
	float d[2];
	float lambda[3];
	float r[3];
	for (int i = 0; i <= 1; i++)
	{
		d[i] = vecLng(p[i + 1], p[i]);
		//std::cout << d[i] << endl;


	}
	//std::cout << endl;
	float dist;
	dist = vecLng(p[0], dest); //Дистанция между корнем и целью

	//std::cout << dist << endl;

	if (dist > (d[0] + d[1])) //Проверяем достижимость цели
	{
		//цель недостижима
		for (int i = 0; i <= 1; i++)
		{
			std::cout << "..." << endl;
			//Найдем дистанцию r[i] между целью t и узлом p[i]
			r[i] = vecLng(dest, p[i]);
			lambda[i] = d[i] / r[i];
			//Находим новую позицию узла p[i]
			// p[i+1] = (1 - lambda[i]) * p[i] + lambda[i] * t
			p[i + 1] = sum(multVEC((1 - lambda[i]), p[i]), multVEC(lambda[i], dest));
		}
	}
	else
	{
		std::cout << "***" << endl;
		//Дель достижима; т.о. b будет новой позицией узла p[0]

			//Проверяем, не выше ли дистанция между конечным узлом p[n] и 
			//целевой позицией t значения терпимости (tolerance)
		float DIFa = vecLng(p[2], dest);
		while (DIFa > 0.1)
		{
			//std::cout << "3 ----------" << endl;
			//Этап 1 : прямое следование
			//Устанавливаем конечный узел p[n] в качестве цели (вероятно, имелось ввиду "ставим на позицию цели" - прим. перев.)
			p[2] = dest;
			//out(p[2]);
			for (int i = 1; i >= 0; i--)
			{
				//Получаем расстояние r[i] между узлом p[i] и новой позицией p[i+1]
				r[i] = vecLng(p[i], p[i + 1]);
				lambda[i] = d[i] / r[i];
				//Вычисляем новую позицию  узла p[i]

				// p[i] = ( 1 - lambda[i]) * p[i+1] + lambda[i] * p[i]
				p[i] = sum(multVEC((1 - lambda[i]), p[i + 1]), multVEC(lambda[i], p[i]));
				//out(p[i]);
			}


			//Этап 2: обратное следование
			//Устанавливаем корневому элементу p[0] начальную позицию
			p[0] = MVector::toPoint(0);
			for (int i = 0; i <= 1; i++)
			{
				//Получаем дистанцию r[i] между узлом p[i+1] и позицией p[i]
				r[i] = vecLng(p[i + 1], p[i]);
				lambda[i] = d[i] / r[i];
				//Получаем новую позицию p[i]

			//p[i + 1] = (1 - lambda[i]) * p[i] + lambda[i] * p[i + 1]
				p[i + 1] = sum(multVEC((1 - lambda[i]), p[i]), multVEC(lambda[i], p[i + 1]));
				//out(p[i]);
			}

			DIFa = vecLng(p[2], dest);
			//out(DIFa);
		}
	}

	solved.p[0].x = p[0].x;
	solved.p[1].x = p[1].x;
	solved.p[2].x = p[2].x;

	solved.p[0].y = p[0].y;
	solved.p[1].y = p[1].y;
	solved.p[2].y = p[2].y;

	solved.p[0].z = p[0].z;
	solved.p[1].z = p[1].z;
	solved.p[2].z = p[2].z;

	return solved;
}



angles converter(pM solved)
{
	angles n;
	n.angle[0] = 180 / M_PI * atan2f(solved.p[1].y, solved.p[1].x);
	n.angle[1] = 180 / M_PI * atan2f(dif(solved.p[2], solved.p[1]).y, dif(solved.p[2], solved.p[1]).x);

	n.rootAngle = 180 / M_PI * atan2f(solved.p[1].z, solved.p[1].y);
	return n;

}



float readFloat(string line, int i)
{
	i++;
	string out;
	while (true)
	{
		if (i > line.length()) { break; }
		if (line[i] == ' ') { break; }

		out += line[i];

		i++;
	}
	return stof(out);

}



int eeprom_cell_num = 0; //счетчик ячеек еепрома

void eeprom_settings(uint8_t eeprom[]){ // начальные настройки руки

	eeprom[0] = settings.speedCOM;
	eeprom[1] = settings.speedDAT;

	eeprom_cell_num = 2; 
}



uint8_t* move_arr(uint8_t *arr,int distance){
	
	if (distance < 0) {
		distance = -distance;
		goto here;
	}
	for (int i = 0; i < distance; i++) {
		*arr++;
	}
	return arr;
here:for (int i = 0; i < distance; i++) {
		*arr--;
	}
	return arr;
}


angles last_angles;

void eeprom_writer( uint8_t *eeprom, unsigned int command ,data_E data) {
		
	int last_cell = eeprom_cell_num;		// записываем номер ячейки начала этой команды

	

	if (eeprom_cell_num >=1024 ) {			// проверяем нет ли переполнения
		return;						
	}

	eeprom = move_arr(eeprom,eeprom_cell_num);				//доходим до начала этой команды

	if (command == move) {							// если двигаем 

		uint8_t controll_byte = command;	//контрольный байт


		eeprom_cell_num++;		// оставляем место для контрольного байта
		*eeprom++;
		

		if (data.angle.angle[0] == last_angles.angle[0]) {
			controll_byte &= 0b11111101;							// проверяем 1 угол
		}
		else {
			*eeprom++ = roundf(data.angle.angle[0]);					//записываем 1 угол 
			eeprom_cell_num++;										// счетчик ячеек +1
		}

		if (data.angle.angle[1] == last_angles.angle[1]) {
			controll_byte &= 0b11111110;							// проверяем 2 угол
		}
		else {
			*eeprom++ = roundf(data.angle.angle[1]);
			eeprom_cell_num++;										// счетчик ячеек +1
		}

		if (data.angle.rootAngle == last_angles.rootAngle) {
			controll_byte &= 0b11111011;							// проверяем корневой угол
		}
		else {
			*eeprom++ = roundf(data.angle.rootAngle);
			eeprom_cell_num++;										// счетчик ячеек +1
		}

		eeprom = move_arr(eeprom, -(eeprom_cell_num - last_cell));	// откатываемся до начала команды для записи 
																	//контрольного байта 
		*eeprom++ = controll_byte;									//и записываем в еепром контрольный байт
			
		last_angles = data.angle;


	} 
	else if (command == wait) {				// если ждем
		uint8_t controll_byte = command;	//контрольный байт
		*eeprom++ = command;
		eeprom_cell_num++;
		*eeprom++ = data.wait_s;
		eeprom_cell_num++;
	} 
	



}



int main()
{
	
	uint8_t EEPROM[1024];// память...
	pM cords;			//корды для углов
	angles angles;		//углы
	string path;		//путь
	data_E data;		//данные ( по другому никак... точнее как но мне лень ) 

	while (true) {
		std::cout << "enter file path\n";
		//std::cin >> path;
		std::cout << path;
		path = "F:\\c++_projects\\test_data.txt";		//шоб не напрягатся
		ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "error,incorect path!!" << endl << endl;	//дебил куда лезеш
		}
		else
		{
			break;
		}
	}
	eeprom_settings(EEPROM);
	string line;
	char Cline[50];
	ifstream file(path);

	while (!file.eof())
	{
		getline(file, line);
		std::cout << "data: " << line << endl;
	}
	file.clear();
	file.seekg(0, ios::beg);
	while (getline(file, line))
	{
		
		for (int i = 0; i <= line.length(); i++)
		{
			char s = line[i];

			switch (s)
			{
			case 'X':
				cout << "X" << readFloat(line, i) << endl;
				dest.x = readFloat(line, i);
			break;

			case 'Y':
				cout << "Y" << readFloat(line, i) << endl;
				dest.y = readFloat(line, i);
			break;

			case 'Z':
				cout << "Z" << readFloat(line, i) << endl;
				dest.z = readFloat(line, i);
			break;

			case 'W':		//тут и без S можно
				cout << "W" << readFloat(line, i) << endl;
				data.wait_s = readFloat(line, i);
				eeprom_writer(EEPROM, wait, data);
				
			break;
			case 'S':			// решаем координаты

				cords = solver(dest);
				angles = converter(cords);
				data.angle = angles;
				eeprom_writer(EEPROM,move,data);
				cout << "++++++++" << endl;
				cout << angles.angle[0] << endl;
				cout << angles.angle[1] << endl;
				cout << angles.rootAngle << endl;
				cout << "++++++++" << endl;
			break;
			}
		}
	}

	file.close();
	char ComPortName[] = "\\\\.\\COM4";
	USB::write(ComPortName,EEPROM);
	
	int vol;
	/*
	cout << "------" << endl;
	cout << "------" << endl;
	cout << dest.x << endl;
	cout << dest.y << endl;
	cout << dest.z << endl;
	cout << "------" << endl;
	cout << cords.p[1].x << endl;
	cout << cords.p[1].y << endl;
	cout << cords.p[1].z << endl;
	cout << "------" << endl;
	cout << cords.p[2].x << endl;
	cout << cords.p[2].y << endl;
	cout << cords.p[2].z << endl;
	cout << "_______" << endl;
	cout << angles.angle[0] << endl;
	cout << angles.angle[1] << endl;
	cout << angles.rootAngle << endl;
	cout << "------" << endl;
	cout << "------" << endl;
	*/
	cout << "eeprom" << endl;

	for (int i = 0; i <= 20; i++) {
		
		cout << std::bitset<sizeof(EEPROM[i]) * CHAR_BIT>(EEPROM[i])  <<"|"<< int(EEPROM[i]) << endl;
	}

	cin >> vol;

}