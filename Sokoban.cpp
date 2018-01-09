#include <iostream>
#include <sstream>
#include <fstream>
#include <conio.h>
using namespace std;

//Fungsi menampilkan main menu
void MainMenu();

//Fungsi menampilkan pilihan level
void SelectLevel();

//Fungsi bermain
void Play();

//Fungsi menampilkan map
void DisplayMap();

//Fungsi kontrol inputan dari user
void control(char key);

//Fungsi player bergerak
void move();

//Fungsi yang mendeteksi aksi ketika player bergerak, seperti menyentuh tembok, mendorong kotak, dll
void actionTrigger();

//Fungsi mengecek apakah sudah menang atau belum
bool winCheck();

//Fungsi untuk melakukan undo bergerak
void Undo();

//Fungsi menyimpan langkah sebelumnya ke daftar undo
void updateUndo();

//Fungsi menampilkan logo sokoban
void printLogo();

//Fungsi menampilkan judul level ketika bermain
void printLevelTitle();

//Fungsi melakukan reset progress level
void ResetProgress();

//Fungsi menyimpan progress ketika bermain
void SaveProgress();

//Fungsi menginisialisasi map ketika user memilih level
void InitMap();

int x = 1; //Posisi player di koordinat x
int y = 1; //Posisi player di koordinat y
int v_x = 0; //Ketika player bergerak di koordinat x
int v_y = 0; //Ketika player bergerak di koordinat y
int p_x = 11; //Panjang x map
int p_y = 11; //Panjang y map
int level; //Pilihan level user
int total_move = 0; //Total player melakukan gerakan di satu level
char inputControl; //Input keyboard yg dimasukkan user
char currentMap[11][11]; //Map level yang sedang dimainkan

//Struktur dari map
struct Map {
	int x; //Posisi player di x koordinat
	int y; //Posisi player di y koordinat
	int best_move; //Best move untuk mendapatkan 3 bintang
	char area[11][11]; //Mapnya
};

//Struktur dari undo
struct Undos {
	int x; //Posisi player di x koordinat
	int y; //Posisi player di y koordinat
	char area[11][11]; //Mapnya
};

//Ini inisialisasi daftar undo yang dapat dilakukan player
Undos undos[5] = {{'Z'}, {'Z'}, {'Z'}, {'Z'}, {'Z'}};

//Inisialisasi ketiga map
Map maps[3] = {
		{5, 2, 35, {
			"  [===]   ",
			"  |   |   ",
			"[=)KKP|   ",
			"|   ==>   ",
			"|     |   ",
			"| x x |   ",
			"(=====)   ",
			"          ",
			"          ",
			"          ",
			"          "
		}},
	
		{4, 1, 47, {
			"  [==]    ",
			"  | P(=]  ",
			"  |KK  |  ",
			"[=) Kx |  ",
			"|xxx [=)  ",
			"(=]K |    ",
			"  |  |    ",
			"  (==)    ",
			"          ",
			"          ",
			"          "
		}},
		
		{7, 7, 70, {
			"[===]     ",
			"|   (===] ",
			"| K K K | ",
			"(=] | = | ",
			"  | |   | ",
			" [) (== (]",
			" | xxxxx |",
			" | K K P |",
			" | === [=)",
			" |     |  ",
			" (=====)  "
		}},
	};

main() {
	//Memanggil mainmenu ketika pertama kali membuka game
	MainMenu();
}

/*
	Fungsi menginisialisasi map ketika user memilih level
	Proses: mengcopy isi map sesuai level yang dipilih user, dan dimasukkan ke array currentMap
*/
void InitMap() {
	x = maps[level - 1].x;
	y = maps[level - 1].y;
	
	for(int i=0; i<p_y; i++){
		for(int j=0; j<p_x; j++){
			currentMap[i][j] = maps[level - 1].area[i][j];
		}
	}
}

//Menampilkan logo
void printLogo() {
	cout<<"#####################################################"<<endl;
	cout<<"#####################################################"<<endl;
	cout<<"##      #      # ###  #      #      ##    ##  #### ##"<<endl;
	cout<<"## ###### #### # ## ### #### # #### # #### # # ### ##"<<endl;
	cout<<"##      # #### #   #### #### #     ##      # ## ## ##"<<endl;
    cout<<"####### # #### # ## ### #### # #### # #### # ### # ##"<<endl;
	cout<<"##      #      # ###  #      #      # #### # ####  ##"<<endl;
	cout<<"#####################################################"<<endl;
	cout<<"#####################################################"<<endl<<endl;
}

//Menampilkan title level
void printLevelTitle() {
	cout<<"=======================\n";
	cout<<"=== SOKOBAN LEVEL "<<level<<" ===\n";
	cout<<"=======================\n\n";
}

void MainMenu() {
	system("cls");
	
	printLogo();
	
	int pil;
	
	cout<<"=== SELAMAT DATANG DI SOKOBAN ==="<<endl;
	cout<<"1. Bermain\n";
	cout<<"2. Reset Data\n";
	cout<<"3. Keluar\n\n";
	
	SELECT:
	cout<<"Masukkan pilihan: ";
	cin>>pil;
	
	if(pil == 1)
		SelectLevel();
	else if(pil == 2) {
		ResetProgress();
		goto SELECT;
	}
	else if(pil == 3)
		exit(0);
}

void ResetProgress() {
	//Ask user to confirmation
	char s;
	cout<<"\nApakah anda yakin ingin mereset data? (y/n): ";
	cin>>s;
	
	if(s == 'y') {
		//Reset
		ofstream myfile ("SAVE_DATA");
		if(myfile.is_open()) {
			myfile<<"U=0=0\nL=0=0\nL=0=0"<<endl;
			
			myfile.close();
		}
		cout<<"Reset data berhasil!\n\n";
	}
	else
		cout<<"Reset data dibatalkan!\n\n";
}

void SelectLevel() {
	system("cls");
	ifstream file ("SAVE_DATA");
	string line;
	char status[3];
	
	printLogo();
	
	cout<<"=== PILIH LEVEL ==="<<endl;
	cout<<"0. Main menu\n";
	
	int i = 1;
	if(file.is_open()) {
		while(getline(file, line)) {
			status[i-1] = line[0];
			
			cout<<i<<". Level "<<i;
			if(line[0] == 'L')
				cout<<" (Terkunci)";
			else
				cout<<" ("<<line[2]<<" star)";
			
			cout<<endl;
			i++;
		}
		
		while(true) {
			cout<<"\nMasukkan pilihan level: ";
			cin>>level;
			
			if(level == 0)
				MainMenu();
			else if(status[level-1] == 'L')
				cout<<"Level belum terbuka!"<<endl;
			else if(level < 1 || level > 3)
				cout<<"Input level tidak valid!"<<endl;
			else
				break;
		}
		
		file.close();
	}
	
	Play();
}

void Play() {	
	//Reset Undo
	undos[0].area[0][0] = 'Z';
	undos[1].area[0][0] = 'Z';
	undos[2].area[0][0] = 'Z';
	undos[3].area[0][0] = 'Z';
	undos[4].area[0][0] = 'Z';
	
	total_move = 0;
	
	InitMap();
	
	system("cls");
	printLevelTitle();
	
	DisplayMap();
	
	//Get player control key pressed
	while(!winCheck()) {
		if(kbhit()) {
			inputControl = getch();
			
			if(inputControl == 'q' || inputControl == 'Q')
				break;
			
			if(inputControl != 'w' && inputControl != 'W' && inputControl != 'a' && inputControl != 'A' && inputControl != 's' && inputControl != 'S' && inputControl != 'd' && inputControl != 'D' && inputControl != 'r' && inputControl != 'R' && inputControl != 'z' && inputControl != 'Z')
				continue;
			
			control(inputControl);
			
			//Render the arena
			system("cls");
			printLevelTitle();
			DisplayMap();
		}
	}
	
	//Jika quit
	if(inputControl == 'q' || inputControl == 'Q')
		MainMenu();
	else {
		//Save progress
		SaveProgress();
		cout<<"\n\nAnda berhasil menyelesaikan level ini.\n";
		cout<<"Ketik sembarang untuk kembali ke pemilihan level\n";
		system("pause");
	
		SelectLevel();	
	}
}

void SaveProgress() {
	ifstream file ("SAVE_DATA");
	string line, lines[3];
	
	int i = 0;
	if(file.is_open()) {
		stringstream ss;
		int star = 0;
		int best = maps[level - 1].best_move;
		
		if(total_move <= best)
			star = 3;
		else if(total_move <= best + 10)
			star = 2;
		else
			star = 1;
		
		while(getline(file, line)) {
			if(i == level - 1)
				ss<<"U="<<star<<"="<<total_move;
			else if(i == level)
				ss<<"U=0=0";
			else
				ss<<line;
			
			lines[i] = ss.str();
			ss.str("");
			i++;
		}
		
		file.close();
	}
	
	//Saving
	ofstream myfile ("SAVE_DATA");
	if(myfile.is_open()) {
		myfile<<lines[0]<<endl;
		myfile<<lines[1]<<endl;
		myfile<<lines[2]<<endl;
		
		myfile.close();
	}
}

bool winCheck() {
	bool winStatus = true;
	
	for(int i = 0; i < p_y; i++) {
		for(int j = 0; j < p_x; j++) {
			if(maps[level - 1].area[i][j] == 'x' && currentMap[i][j] != 'K')
				return false;
		}
	}
	
	return winStatus;
}

void DisplayMap() {	
	for(int i = 0; i < p_y; i++){
		for(int j = 0; j < p_x; j++){
			char t = currentMap[i][j];
			
			if(t == '=')
				cout<<char(205);
			else if(t == '|')
				cout<<char(186);
			else if(t == '#')
				cout<<char(206);
			else if(t == '^')
				cout<<char(203);
			else if(t == '<')
				cout<<char(204);
			else if(t == '>')
				cout<<char(185);
			else if(t == '%')
				cout<<char(202);
			else if(t == '[')
				cout<<char(201);
			else if(t == ']')
				cout<<char(187);
			else if(t == '(')
				cout<<char(200);
			else if(t == ')')
				cout<<char(188);
			else if(t == 'K')
				cout<<char(219);
			else
				cout<<t;
		}
		cout<< endl;
	}
	
	cout<<"Move: "<<total_move<<" moves\n";
	cout<<"3 Star Move: "<<maps[level - 1].best_move<<" moves\n";
	cout<<"Controls: \n";
	cout<<"W : to move top\n";
	cout<<"A : to move left\n";
	cout<<"S : to move bottom\n";
	cout<<"D : to move right\n";
	cout<<"R : to restart\n";
	cout<<"Z : to undo last move\n";
	cout<<"Q : to back to select level";
}

void control(char key) {
	//Reset
	v_x = 0;
	v_y = 0;
	
	//Move to top
	if(key == 'W' || key == 'w')
		v_y = -1;
	//Move to left
	else if(key == 'A' || key == 'a')
		v_x = -1;
	//Move to bottom
	else if(key == 'S' || key == 's')
		v_y = 1;
	//Move to right
	else if(key == 'D' || key == 'd')
		v_x = 1;
	//If restart
	else if(key == 'r' || key == 'R')
		Play();
	//If undo
	else if(key == 'z' || key == 'Z')
		Undo();
	
	move();
}

void move() {
	if(v_x != 0 || v_y != 0) {
		//Obstacle check
		if(y + v_y < 0 || x + v_x < 0)
			return;
			
		char t = currentMap[y + v_y][x + v_x];
		if(t == '=' || t == '|' || t == '^' || t == '%' || t == '<' || t == '>' || t == '[' || t == ']' || t == '(' || t == ')') {
			return;
		}
		
		//Call what event will be trigger
		actionTrigger();
	}
}

void actionTrigger() {
	char nextChar = currentMap[y + v_y][x + v_x];
	
	//Event if nothing happen, just move
	if(nextChar == 'K') {
		//Move box to bottom
		//Check if next of bottom not obstacle and move box and player
		if(currentMap[y + v_y + v_y][x + v_x + v_x] == ' ' || currentMap[y + v_y + v_y][x + v_x + v_x] == 'x') {
			//Before move, save to undo history
			updateUndo();
			
			currentMap[y + v_y + v_y][x + v_x + v_x] = nextChar;
			
			//Jika yg dipijaki
			if(maps[level - 1].area[y][x] == 'x')
				currentMap[y][x] = 'x'; //Clear
			else
				currentMap[y][x] = ' '; //Clear
			
			x = x + v_x;
			y = y + v_y;
		
			currentMap[y][x] = 'P'; //Move
			
			total_move++;
		}
	}
	else {
		//Before move, save to undo history
		updateUndo();
		
		//Jika yg dipijaki adalah x
		if(maps[level - 1].area[y][x] == 'x')
			currentMap[y][x] = 'x'; //Clear
		else
			currentMap[y][x] = ' '; //Clear
		
		x = x + v_x;
		y = y + v_y;
		
		currentMap[y][x] = 'P'; //Move
		
		total_move++;
	}
}

void updateUndo() {
	//Arrange undo history position
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < p_y; j++) {
			for(int k = 0; k < p_x; k++) {
				undos[i].area[j][k] = undos[i+1].area[j][k];
			}	
		}
		undos[i].x = undos[i+1].x;
		undos[i].y = undos[i+1].y;
	}
	
	//Add new undo history
	undos[4].x = x;
	undos[4].y = y;
	for(int i = 0; i < p_y; i++) {
		for(int j = 0; j < p_x; j++) {
			undos[4].area[i][j] = currentMap[i][j];
		}
	}
}

void Undo() {
	//If can undo
	if(undos[4].area[0][0] != 'Z') {
		total_move--;
		
		//Update current map
		x = undos[4].x;
		y = undos[4].y;
		for(int i = 0; i < p_y; i++) {
			for(int j = 0; j < p_x; j++) {
				currentMap[i][j] = undos[4].area[i][j];
			}
		}
		
		//Call actionTrigger
		//actionTrigger();
		
		//Arrange undo history position
		for(int i = 4; i > 0; i--) {
			for(int j = 0; j < p_y; j++) {
				for(int k = 0; k < p_x; k++) {
					undos[i].area[j][k] = undos[i-1].area[j][k];
				}	
			}
			undos[i].x = undos[i-1].x;
			undos[i].y = undos[i-1].y;
		}
		
		//Set Z to last item
		undos[0].area[0][0] = 'Z';
	}
}
