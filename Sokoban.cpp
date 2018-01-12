/*
=== GAME SOKOBAN C++ ===
Permainan sokoban objectivenya yaitu mendorong kotak untuk menutupi area x di setiap levelnya.
Dipermainan ini progress selalu tersimpan, dan diwajibkan menyelesaikan level sebelumnya untuk membuka level berikutnya.
Kontrol pemain yaitu:
	W = bergerak ke atas
	A = bergerak ke kiri
	S = bergerak ke bawah
	D = bergerak ke kanan
	R = melakukan restart
	Z = melakukan undo move
	Q = keluar dari level
Setelah berhasil menyelesaikan level, sistem akan menghitung berapa kali melakukan move dalam level tersebut, dan mengubahnya menjadi
range bintang 1 sampai 3 sebagai high scorenya.

Didevelop oleh:
	- Darmawan Saputra
	- Trisna Lestari Tanjung
	- Erica Faradina Putri
*/

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
	cout<<"#####################################################"<<endl;
	cout<<"Didevelop oleh: Darmawan, Nana, dan Erieca"<<endl<<endl<<endl;
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
	
	//Jika pilihan pemain = 1, memanggil fungsi SelectLevel()
	if(pil == 1)
		SelectLevel();
	//Jika pilihan pemain = 2, memanggil fungsi ResetProgress()
	else if(pil == 2) {
		ResetProgress();
		goto SELECT;
	}
	//Jika pilihan pemain = 3, maka keluar
	else if(pil == 3)
		exit(0);
}

void ResetProgress() {
	//Ask user to confirmation
	char s;
	cout<<"\nApakah anda yakin ingin mereset data? (y/n): ";
	cin>>s;
	
	//Jika pemainnya memilih y, maka data akan direset	
	if(s == 'y') {
		//Membuka file bernama SAVE_DATA
		ofstream myfile ("SAVE_DATA");
		
		//Jika berhasil membuka file
		if(myfile.is_open()) {
			//Isi file SAVE_DATA diubah isinya seperti dibawah
			myfile<<"U=0=0\nL=0=0\nL=0=0"<<endl;
			
			//Jika sudah, file ditutup
			myfile.close();
		}
		cout<<"Reset data berhasil!\n\n";
	}
	//Jika tidak jadi mereset data
	else
		cout<<"Reset data dibatalkan!\n\n";
}

//Fungsi menampilkan level level yg ada
void SelectLevel() {
	system("cls");
	
	//Membuka file SAVE_DATA
	ifstream file ("SAVE_DATA");
	string line;
	char status[3];
	
	printLogo();
	
	cout<<"=== PILIH LEVEL ==="<<endl;
	cout<<"0. Main menu\n";
	
	int i = 1;
	if(file.is_open()) {
		//Mengambil isi file baris per baris
		while(getline(file, line)) {
			//Mengambil karakter pertama L atau U
			status[i-1] = line[0];
			
			cout<<i<<". Level "<<i;
			//Jika karakter pertama itu L
			if(line[0] == 'L')
				cout<<" (Terkunci)";
			//Jika karakter pertama itu U
			else
				cout<<" ("<<line[2]<<" star)";
			
			cout<<endl;
			i++;
		}
		
		while(true) {
			//Menanyakan pemain memilih level berapa
			cout<<"\nMasukkan pilihan level: ";
			cin>>level;
			
			//Jika memilih 0, maka ke main menu
			if(level == 0)
				MainMenu();
			//Jika memilih level yg karakter pertamanya L
			else if(status[level-1] == 'L')
				cout<<"Level belum terbuka!"<<endl;
			//Jika memilih level kurang dari 1 atau lebih dari 3
			else if(level < 1 || level > 3)
				cout<<"Input level tidak valid!"<<endl;
			else
				break;
		}
		
		file.close();
	}
	
	//Jika semua valid, panggil fungsi Play()
	Play();
}

void Play() {	
	//Reset Undo
	undos[0].area[0][0] = 'Z';
	undos[1].area[0][0] = 'Z';
	undos[2].area[0][0] = 'Z';
	undos[3].area[0][0] = 'Z';
	undos[4].area[0][0] = 'Z';
	
	//Mereset total movenya
	total_move = 0;
	
	//Proses mengcopy isi map
	InitMap();
	
	system("cls");
	
	printLevelTitle();
	
	DisplayMap();
	
	//Jika winCheck belum menang
	while(!winCheck()) {
		
		//Jika pemain menekan tombol
		if(kbhit()) {
			//Mendapatkan keyboard yg ditekan pemain
			inputControl = getch();
			
			//Jika menekan tombol q
			if(inputControl == 'q' || inputControl == 'Q')
				break;
			
			//Jika pemain menekan tombol selain W A S D Z R maka sistem tidak akan memproses inputan tersebut
			if(inputControl != 'w' && inputControl != 'W' && inputControl != 'a' && inputControl != 'A' && inputControl != 's' && inputControl != 'S' && inputControl != 'd' && inputControl != 'D' && inputControl != 'r' && inputControl != 'R' && inputControl != 'z' && inputControl != 'Z')
				continue;
			
			//Memanggil fungsi control sesuai keyboard yg ditekan
			control(inputControl);
			
			//Setelah bergerak, akan menghapus tampilan yg lama, dan menampilkan yg baru setelah bergerak
			system("cls");
			printLevelTitle();
			DisplayMap();
		}
	}
	
	//Jika quit
	if(inputControl == 'q' || inputControl == 'Q')
		SelectLevel();
	else {
		//Kalau menang, simpan progress
		SaveProgress();
		cout<<"\n\nAnda berhasil menyelesaikan level ini.\n";
		cout<<"Ketik sembarang untuk kembali ke pemilihan level\n";
		system("pause");
	
		SelectLevel();	
	}
}

//menyimpan progress ketika sudah menyelesaikan permaian
void SaveProgress() {
	ifstream file ("SAVE_DATA");
	string line, lines[3];
	
	int i = 0;
	if(file.is_open()) {
		stringstream ss;
		int star = 0;
		int best = maps[level - 1].best_move;
		
		//Proses menetukan pemain mendapatkan bintang berapa setelah menyelesaikan permaianan
		
		//Jika total langkah pemain <= best maka mendapatkan bintang 3
		if(total_move <= best)
			star = 3;
		//Jika total langkah pemain <= best + 10, mendapatkan bintang 2
		else if(total_move <= best + 10)
			star = 2;
		//Jika tidak mendapatkan bintang 1
		else
			star = 1;
		
		//Menggunakan while, untuk mengetahui baris yg perlu dirubah
		while(getline(file, line)) {
			//Mengecek apakah bintang yg didapat di level itu, lebih besar dengan bintang yg sebelumnya sudah didapat di level yg sama
			if(i == level - 1 && star > line[2] - '0')
				ss<<"U="<<star<<"="<<total_move;
			//Jika level selanjut statusnya Locked, maka buka level tersebut, karena sudah menyelesaikan level sebelumnya
			else if(i == level && line[0] == 'L')
				ss<<"U=0=0";
			else
				ss<<line;
			
			lines[i] = ss.str();
			ss.str("");
			i++;
		}
		
		file.close();
	}
	
	//Proses menyimpan
	ofstream myfile ("SAVE_DATA");
	if(myfile.is_open()) {
		myfile<<lines[0]<<endl;
		myfile<<lines[1]<<endl;
		myfile<<lines[2]<<endl;
		
		myfile.close();
	}
}

//Mengecek apakah sudah menang atau belum, dengan cara memproses apakah tanda X sudah ditutupi semua oleh tanda K
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
		if(currentMap[i][0] == ' ' && currentMap[i][1] == ' ' && currentMap[i][2] == ' ' && currentMap[i][3] == ' ' && currentMap[i][4] == ' ' && currentMap[i][5] == ' ' && currentMap[i][6] == ' ' && currentMap[i][7] == ' ' && currentMap[i][8] == ' ' && currentMap[i][9] == ' ')
			break;
			
		for(int j = 0; j < p_x; j++){
			char t = currentMap[i][j];
			
			//Proses merubah karakter tiap map, agar bagus
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
	
	cout<<"\nMove: "<<total_move<<" moves\n";
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

//Fungsi kontrol dari keyboard, sesuai tombol yang ditekan pemain
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
	
	//Memanggil fungsi untuk bergerak
	move();
}

void move() {
	//Jika bergerak, selain menekan W A S D
	if(v_x != 0 || v_y != 0) {
		//Mengecek apakah bisa bergerak atau tidak, apakah dipojok atau tidak
		if(y + v_y < 0 || x + v_x < 0)
			return;
		
		//Dicek apakah pergerkannya itu menabrak tembok
		char t = currentMap[y + v_y][x + v_x];
		if(t == '=' || t == '|' || t == '^' || t == '%' || t == '<' || t == '>' || t == '[' || t == ']' || t == '(' || t == ')') {
			return;
		}
		
		//Memanggil fungsi actionTrigger()
		actionTrigger();
	}
}

//Fungsi untuk mendeteksi aksi apa yang terjadi, seperti mendorong kotak, bergerak, membuat tanda x kembali ketika mendorok kotak keluar dari x
void actionTrigger() {
	char nextChar = currentMap[y + v_y][x + v_x];
	
	//Jika karakter selanjutnya itu Kotak, aksi ketika mendorong kotak
	if(nextChar == 'K') {
		//Move box to bottom or right or top or left
		//Karakter selanjutnya setelah kotak itu jika spasi, maka bisa bergerak
		if(currentMap[y + v_y + v_y][x + v_x + v_x] == ' ' || currentMap[y + v_y + v_y][x + v_x + v_x] == 'x') {
			//Sebelum berpindah, simpan dulu pergerakan sebelumnya ke daftar undo
			updateUndo();
			
			//Mengubah karakter tujuan dengan P atau K
			currentMap[y + v_y + v_y][x + v_x + v_x] = nextChar;
			
			//Jika karakter yg ditutupi itu X
			if(maps[level - 1].area[y][x] == 'x')
				currentMap[y][x] = 'x'; //Clear
			//Jika bukan x
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

//Menambahkan langkah terakhir ke array terakhir undo, agar bisa diambil ketika melakukan proses undo
void updateUndo() {
	//Mengurutkan undonya
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < p_y; j++) {
			for(int k = 0; k < p_x; k++) {
				undos[i].area[j][k] = undos[i+1].area[j][k];
			}	
		}
		undos[i].x = undos[i+1].x;
		undos[i].y = undos[i+1].y;
	}
	
	//Menambahkan ke undo terakhir
	undos[4].x = x;
	undos[4].y = y;
	for(int i = 0; i < p_y; i++) {
		for(int j = 0; j < p_x; j++) {
			undos[4].area[i][j] = currentMap[i][j];
		}
	}
}

//Fungsi melakukan proses undo
void Undo() {
	//Jika bisa melakukan undo
	if(undos[4].area[0][0] != 'Z') {
		total_move--;
		
		//mengambil posisi history yang terakhir dan dijadikan map yang sekarang
		x = undos[4].x;
		y = undos[4].y;
		for(int i = 0; i < p_y; i++) {
			for(int j = 0; j < p_x; j++) {
				currentMap[i][j] = undos[4].area[i][j];
			}
		}
		
		//Mengurukan undo, karena posisi yang terakhir sudah diambil, maka posisi terakhir digantikan posisi yang ada di depannya
		for(int i = 4; i > 0; i--) {
			for(int j = 0; j < p_y; j++) {
				for(int k = 0; k < p_x; k++) {
					undos[i].area[j][k] = undos[i-1].area[j][k];
				}	
			}
			undos[i].x = undos[i-1].x;
			undos[i].y = undos[i-1].y;
		}
		
		//Ubah undo pertama menjadi z, untuk menandakan limit undonya
		undos[0].area[0][0] = 'Z';
	}
}
