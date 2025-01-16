#include <iostream>
#include <string>
using namespace std;

struct kamera {
  string merek;
  string tipe;
  int harga;
  struct kamera *next;
  struct kamera *prev;
};

struct lensa {
  string merek;
  string tipe;
  int harga;
  struct lensa *next;
  struct lensa *prev;
};

struct member {
  int id;
  string nama_member;
  string tanggal_lahir;
  string no_telp;
  struct member* parent;
  struct member *left;
  struct member *right;
};

// Definisi struktur fotografer
struct fotografer {
  string nama;
  string umur;
  char kelamin;
  string pengalaman;
  int harga;
  fotografer *left;
  fotografer *right;
};

// Definisi struktur Node Stack
struct StackNode {
  fotografer data;
  StackNode *next;
};

// Definisi struktur Stack
struct Stack {
  StackNode *top;
};

struct destinasi {
    string tempat;
    destinasi* next;
};

struct graph {
    int banyak_tempat;
    string tempat[10];
    destinasi* adj[10];
};

struct penyewaan_barang{
  int id;
  string nama;
  kamera *kamera_yang_disewa;
  lensa *lensa_yang_disewa;
  int harga_total;
  int hari;
  penyewaan_barang *next;
};

// Fungsi untuk menampilkan pesan selamat datang
void welcome() {
  cout << "================================\n";
  cout << "      Welcome to Cam Store\n";
  cout << "================================\n\n";
}

void welcome_admin() {
  cout << "================================\n";
  cout << "            Admin\n";
  cout << "================================\n\n";
}

void welcome_user() {
  cout << "================================\n";
  cout << "              User\n";
  cout << "================================\n\n";
}

// Fungsi untuk admin
void admin_input_kamera();
void admin_input_lensa();
// void admin_proses_pesanan(); // pakai hash table
void admin_hapus_kamera();
void admin_hapus_lensa();
void admin_buat_member();
void admin_lihat_member();      // pakai tree
void admin_lihat_jarak_toko();  // pakai graph buat liat jarak antar toko
void admin_tambah_fotografer(); // pakai stack
void admin_hapus_member(); //hapus node di tree
void admin_urut_kamera();
void admin_urut_lensa();

// Fungsi untuk user
void user_buat_member_untuk_kamera(kamera *kamera_yang_disewa);
void user_pesan_kamera();
void user_pesan_lensa();
void user_cari_kamera();
void user_cari_lensa();
void user_pesan_fotografer(); // pakai stack

// Fungsi lain
void list_kamera();
void list_lensa();
int buat_id(string nama, string tanggal_lahir, string no_telp);
void pop();
void push();
void input_awal_kamera(string, string, int);
void input_awal_lensa(string, string, int);
void hapus_penyewaan(int id_dicari);

// Kumpulan head linked list
kamera *head_kamera = nullptr;
kamera *tail_kamera = nullptr;
lensa *head_lensa = nullptr;
lensa *tail_lensa = nullptr;
member *root = nullptr;


const string kunci = "12345";
const int ukuran_tabel_hash = 10;
penyewaan_barang *tabel_hash[ukuran_tabel_hash] = {nullptr};
void clearScreen() { cout << "\033[2J\033[1;1H"; }


void BFS(graph &tour, string start) {
    bool visited[10] = {false};
    string queue_tour[10];
    int front = 0, rear = 0;

    int start_index = -1;
    for (int i = 0; i < tour.banyak_tempat; i++) {
        if (tour.tempat[i] == start) {
            start_index = i;
            break;
        }
    }

    if (start_index == -1) return;

    visited[start_index] = true;
    queue_tour[rear++] = start;

    while (front < rear) {
        string start_tour = queue_tour[front++];
        cout << start_tour << " ---> ";

        int index_sekarang = -1;
        for (int i = 0; i < tour.banyak_tempat; i++) {
            if (tour.tempat[i] == start_tour) {
                index_sekarang = i;
                break;
            }
        }

        destinasi *adj_list = tour.adj[index_sekarang];
        while (adj_list != nullptr) {
            int adj_index = -1;
            for (int i = 0; i < tour.banyak_tempat; i++) {
                if (tour.tempat[i] == adj_list->tempat) {
                    adj_index = i;
                    break;
                }
            }

            if (!visited[adj_index]) {
                visited[adj_index] = true;
                queue_tour[rear++] = adj_list->tempat;
            }
            adj_list = adj_list->next;
        }
    }
    cout << "Finish\n";
}

void toUpperCase(string &str) {
    for (char &c : str) {
        c = toupper(static_cast<unsigned char>(c));
    }
}

// Fungsi untuk inisialisasi stack
void initStack(Stack &stack) { stack.top = nullptr; }

// Fungsi untuk push elemen ke stack dengan input dari pengguna
void push(Stack &stack) {
  clearScreen();
  StackNode *newNode = new StackNode();

  cout << "Masukkan nama fotografer: ";
  cin.ignore();
  getline(cin, newNode->data.nama);

  cout << "Masukkan umur fotografer: ";
  getline(cin, newNode->data.umur);

  cout << "Masukkan jenis kelamin fotografer (L/P): ";
  cin >> newNode->data.kelamin;
  cin.ignore(); // Ignore newline character left in buffer

  cout << "Masukkan pengalaman fotografer (.. tahun/... bulan): ";
  getline(cin, newNode->data.pengalaman);

  cout << "Masukkan harga jasa (per sesi): ";
  cin >> newNode->data.harga;

  cin.ignore();
  newNode->next = stack.top;
  stack.top = newNode;
  cout << "Fotografer " << newNode->data.nama << " berhasil ditambahkan ke stack\n";
}

// Fungsi untuk pop elemen dari stack
fotografer pop(Stack &stack) {
  if (stack.top == nullptr) {
    cout << "Stack kosong\n";
    return fotografer();
  }
  StackNode *temp = stack.top;
  stack.top = stack.top->next;
  fotografer item = temp->data;
  delete temp;
  return item;
}

// Fungsi untuk menghapus fotografer berdasarkan nama
void deleteByName(Stack &stack, const string &name) {
  StackNode *current = stack.top;
  StackNode *previous = nullptr;

  while (current != nullptr) {
    if (current->data.nama == name) {
      if (previous == nullptr) {
        stack.top = current->next;
      } else {
        previous->next = current->next;
      }
      delete current;
      cout << "Fotografer dengan nama " << name << " telah dihapus\n";
      return;
    }
    previous = current;
    current = current->next;
  }

  cout << "\nFotografer dengan nama " << name << " tidak ditemukan\n";
}

// Fungsi untuk menampilkan fotografer satu per satu
void displayFotografer(StackNode *node) {
  clearScreen();
  if (node != nullptr) {
    cout << "Nama\t: " << node->data.nama << endl
         << "Umur\t: " << node->data.umur << endl
         << "Kelamin\t: " << node->data.kelamin << endl
         << "Pengalaman: " << node->data.pengalaman << endl
         << "Harga jasa (per sesi): " << node->data.harga << endl;
  }
}

void displayStackInteractive(Stack &stack) {
    StackNode *current = stack.top;
    if (current == nullptr) {
        cout << "Stack kosong\n";
        return;
    }

    // Menghitung total node untuk memudahkan navigasi circular
    int totalNode = 0;
    StackNode *tempNode = stack.top;
    while (tempNode != nullptr) {
        totalNode++;
        tempNode = tempNode->next;
    }

    int currentIndex = 0;
    int choice;

    do {
        clearScreen();
        displayFotografer(current);

        cout << "1. Lihat Fotografer Selanjutnya\n";
        cout << "2. Lihat Fotografer Sebelumnya\n";
        cout << "0. Exit\n> ";
        cin >> choice;

        if (choice == 1) {
            if (current->next != nullptr) {
                current = current->next;
                currentIndex++;
            } else {
                // Kembali ke elemen pertama jika mencapai akhir
                current = stack.top;
                currentIndex = 0;
            }
        } else if (choice == 2) {
            if (currentIndex == 0) {
                // Pergi ke elemen terakhir jika di elemen pertama
                StackNode *lastNode = stack.top;
                while (lastNode->next != nullptr) {
                    lastNode = lastNode->next;
                }
                current = lastNode;
                currentIndex = totalNode - 1;
            } else {
                // Pergi ke elemen sebelumnya
                StackNode *prevNode = stack.top;
                for (int i = 0; i < currentIndex - 1; i++) {
                    prevNode = prevNode->next;
                }
                current = prevNode;
                currentIndex--;
            }
        }

    } while (choice != 0);
    clearScreen();
}

void pesan_fotografer (Stack &stack) { // pesan fotografer
  StackNode *current = stack.top;
  StackNode *previous = nullptr;

  if (current == nullptr) {
    cout << "Stack kosong\n";
    return;
  }

  int choice;
  do {
    displayFotografer(current);

    cout << "1. Lihat Fotografer Selanjutnya\n";
    cout << "2. Lihat Fotografer Sebelumnya\n";
    cout << "3. Pesan Fotografer\n"; 
    cout << "0. Exit\n> ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
      if (current->next != nullptr) {
        previous = current;
        current = current->next;
      } else {
        cout << "Sudah di fotografer terakhir\n";
        cin.get();
      }
      clearScreen();
    } else if (choice == 2) {
      if (previous != nullptr) {
        current = previous;
        previous = nullptr; // Reset previous untuk pencarian ulang
        StackNode *temp = stack.top;
        while (temp != current && temp != nullptr) {
          previous = temp;
          temp = temp->next;
        }
      } else {
        cout << "Sudah di fotografer pertama\n";
      }
      clearScreen();
    } else if (choice == 3) {
      cout << "\nFotografer telah dipesan oleh anda\n"; 
      pop(stack);
      return; 
    } else {
      cout << "Item tidak ditemukan\n"; 
    }
  } while (choice != 0);
  clearScreen();
}

// Fungsi untuk memasukkan fotografer awal ke stack
void input_awal_fotografer(Stack &stack, string nama, string umur, char kelamin, string pengalaman, int harga) {
  StackNode *newNode = new StackNode();
  newNode->data.nama = nama;
  newNode->data.umur = umur;
  newNode->data.kelamin = kelamin;
  newNode->data.pengalaman = pengalaman;
  newNode->data.harga = harga;
  newNode->next = stack.top;
  stack.top = newNode;
}

int fungsi_hash(int id){
  return id % ukuran_tabel_hash;
}

void sewa_kamera(int id, string nama, kamera* kamera_yang_disewa_sekarang, int hari, int harga_total){
  int index = fungsi_hash(id);
  penyewaan_barang *entry = tabel_hash[index];

  while (entry != nullptr){
    if(entry->id == id){
      kamera_yang_disewa_sekarang->next = entry->kamera_yang_disewa;
      if(entry->kamera_yang_disewa != nullptr){
        entry->kamera_yang_disewa->prev = kamera_yang_disewa_sekarang;
      }
      entry->kamera_yang_disewa = kamera_yang_disewa_sekarang;
      return;
    }
    entry = entry->next;
  }

  entry = new penyewaan_barang();
  entry->id = id;
  entry->nama = nama;
  entry->kamera_yang_disewa = kamera_yang_disewa_sekarang;
  entry->lensa_yang_disewa = nullptr;
  entry->hari = hari;
  entry->harga_total = harga_total;
  entry->next = tabel_hash[index];
  tabel_hash[index] = entry;

  cin.ignore();
  cout << "\nPermohonan penyewaan telah diajukan, silahkan datang ke tempat kami\n\n";
  return;
}

void sewa_lensa(int id,string nama, lensa* lensa_yang_disewa_sekarang, int hari, int harga_total){
  int index = fungsi_hash(id);
  penyewaan_barang *entry = tabel_hash[index];

  while (entry != nullptr){
    if(entry->id == id){
      lensa_yang_disewa_sekarang->next = entry->lensa_yang_disewa;
      if(entry->lensa_yang_disewa != nullptr){
        entry->lensa_yang_disewa->prev = lensa_yang_disewa_sekarang;
      }
      entry->lensa_yang_disewa = lensa_yang_disewa_sekarang;
      return;
    }
    entry = entry->next;
  }

  entry = new penyewaan_barang();
  entry->id = id;
  entry->nama = nama;
  entry->kamera_yang_disewa = nullptr;
  entry->lensa_yang_disewa = lensa_yang_disewa_sekarang;
  entry->hari = hari;
  entry->harga_total = harga_total;
  entry->next = tabel_hash[index];
  tabel_hash[index] = entry;

  cin.ignore();
  cout << "\nPermohonan penyewaan telah diajukan, silahkan datang ke tempat kami\n\n";
  return;
}

void display_data_penyewaan() {
    bool isEmpty = true;
    for (int i = 0; i < ukuran_tabel_hash; i++) {
        if (tabel_hash[i] != nullptr) {
            isEmpty = false;
            break;
        }
    }

    if (isEmpty) {
        cout << "\nTidak ada data penyewaan\n\n";
        return;
    }

    for (int i = 0; i < ukuran_tabel_hash; i++) {
        penyewaan_barang* entry = tabel_hash[i];
        while (entry != nullptr) {
            cout << entry->nama << " dengan " << entry->id << " meminjam \n";
            if (entry->kamera_yang_disewa != nullptr) { 
                cout << "\tKamera " << entry->kamera_yang_disewa->merek << " " << entry->kamera_yang_disewa->tipe << " selama " << entry->hari << " hari, total biaya Rp " << entry->harga_total << endl << endl;
            }

            if (entry->lensa_yang_disewa != nullptr) {
                cout << "\tLensa " << entry->lensa_yang_disewa->merek  << " " << entry->lensa_yang_disewa->merek  << " selama " << entry->hari << " hari, total biaya Rp " << entry->harga_total << endl << endl;
            }
            entry = entry->next; 
        }
    }
    return;
}

void hapus_penyewaan(int id_dicari) {
    int index = fungsi_hash(id_dicari);
    penyewaan_barang* entry = tabel_hash[index];
    penyewaan_barang* prev = nullptr;

    while (entry != nullptr) {
        if (entry->id == id_dicari) {
            if (prev == nullptr) {
                tabel_hash[index] = entry->next;
            } else {
                prev->next = entry->next;
            }

            delete entry;
            cout << "\nData penyewaan untuk member " << id_dicari << " telah dihapus.\n\n";
            return;
        }
        prev = entry;
        entry = entry->next;
    }

    cout << "\nData penyewaan untuk member " << id_dicari << " tidak ditemukan.\n\n";
}

void tambah_garis(graph &tour, string start, string finish) {
    int start_index = -1, finish_index = -1;
    for (int i = 0; i < tour.banyak_tempat; i++) {
        if (tour.tempat[i] == start) start_index = i;
        if (tour.tempat[i] == finish) finish_index = i;
    }

    if (start_index != -1 && finish_index != -1) {
        destinasi* baru = new destinasi{finish, tour.adj[start_index]};
        tour.adj[start_index] = baru;
    }
}

int main() {

  Stack stack;      
  initStack(stack); 

  input_awal_kamera("Sony", "A6000", 120000);
  input_awal_kamera("Fujifilm", "X A10", 90000);
  input_awal_kamera("Canon", "EOS M10", 80000);

  input_awal_lensa("Sony", "FE 70 200MM F4", 150000);
  input_awal_lensa("Sony", "28-70MM FE F/3.5-5.6", 70000);
  input_awal_lensa("Canon", "55 250MM F2.8", 60000);

  input_awal_fotografer(stack, "Yakobus", "19", 'L', "5 tahun", 200000);
  input_awal_fotografer(stack, "Louis", "20", 'L', "3 tahun", 150000); 
  input_awal_fotografer(stack, "Adriano", "21", 'L', "25 tahun", 200000);


  graph destinasi_foto;
  destinasi_foto.banyak_tempat = 10;
  string destinasi[10] = {
    "Kampung Arab", "Kampung Bulak", "Klenteng Sanggar Agung", "Patung Buddha 4 Rupa",
    "Museum House of Sampoerna", "G-Walk Citraland", "Tugu Pahlawan", 
    "Museum Pahlawan", "Jalan Tunjungan", "Kebun Binatang Surabaya"
  };

  for (int i = 0; i < 10; i++){
    destinasi_foto.tempat[i] = destinasi[i];
    destinasi_foto.adj[i] = nullptr;
  }

  tambah_garis(destinasi_foto, "Kampung Arab", "Museum House of Sampoerna");
  tambah_garis(destinasi_foto, "Kampung Arab", "Kampung Bulak");
  tambah_garis(destinasi_foto, "Kampung Bulak", "Patung Buddha 4 Rupa");
  tambah_garis(destinasi_foto, "Kampung Bulak", "Klenteng Sanggar Agung");
  tambah_garis(destinasi_foto, "Kampung Bulak", "Kampung Arab");
  tambah_garis(destinasi_foto, "Klenteng Sanggar Agung", "Patung Buddha 4 Rupa");
  tambah_garis(destinasi_foto, "Klenteng Sanggar Agung", "Kampung Bulak");
  tambah_garis(destinasi_foto, "Patung Buddha 4 Rupa", "Klenteng Sanggar Agung");
  tambah_garis(destinasi_foto, "Patung Buddha 4 Rupa", "Kampung Bulak");
  tambah_garis(destinasi_foto, "Museum House of Sampoerna", "Kampung Arab");
  tambah_garis(destinasi_foto, "Museum House of Sampoerna", "Tugu Pahlawan");
  tambah_garis(destinasi_foto, "Museum House of Sampoerna", "Museum Pahlawan");
  tambah_garis(destinasi_foto, "Museum House of Sampoerna", "G-Walk Citraland");
  tambah_garis(destinasi_foto, "G-Walk Citraland", "Museum House of Sampoerna");
  tambah_garis(destinasi_foto, "G-Walk Citraland", "Tugu Pahlawan");
  tambah_garis(destinasi_foto, "Tugu Pahlawan", "Museum Pahlawan");
  tambah_garis(destinasi_foto, "Tugu Pahlawan", "Museum House of Sampoerna");
  tambah_garis(destinasi_foto, "Tugu Pahlawan", "G-Walk Citraland");
  tambah_garis(destinasi_foto, "Tugu Pahlawan", "Jalan Tunjungan");
  tambah_garis(destinasi_foto, "Museum Pahlawan", "Tugu Pahlawan");
  tambah_garis(destinasi_foto, "Museum Pahlawan", "Jalan Tunjungan");
  tambah_garis(destinasi_foto, "Museum Pahlawan", "Museum House of Sampoerna");
  tambah_garis(destinasi_foto, "Jalan Tunjungan", "Kebun Binatang Surabaya");
  tambah_garis(destinasi_foto, "Jalan Tunjungan", "Tugu Pahlawan");
  tambah_garis(destinasi_foto, "Jalan Tunjungan", "Museum Pahlawan");
  tambah_garis(destinasi_foto, "Kebun Binatang Surabaya", "Jalan Tunjungan");

  int pilih;
  string password;
  do {
    clearScreen();
    welcome();
    cout << "Login Toko Kamera\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "3. Exit\n";
    cout << "> ";
    cin >> pilih;
    cin.ignore();
    if (pilih == 1) {
      int choose;
      clearScreen();
        cout << "Ketikkan password: "; cin >> password;
        if (password != kunci) {
            cout << "\nPassword tidak sesuai!\n";
            cin.get();
            cin.get();
          break;
        }
      do {
        clearScreen();
        welcome_admin();
        cout << "1. Penyimpanan kamera dan lensa\n";
        cout << "2. Proses penyewaan\n";
        cout << "3. Pengurusan Member\n";
        cout << "4. Fotografer\n";
        cout << "0. Logout\n";
        cout << "> ";
        cin >> choose;
        cin.ignore();
        switch (choose) {
          case 1:{
                int storage_choose;
                cout << "\t1. List Kamera\n";
                cout << "\t2. List Lensa\n";
                cout << "\t3. Input Kamera\n";
                cout << "\t4. Input Lensa\n";
                cout << "\t5. Hapus Kamera\n";
                cout << "\t6. Hapus Lensa\n";
                cout << "\t > ";
                cin >> storage_choose;
                cin.ignore();

                switch(storage_choose){
                  case 1: {
                    list_kamera();
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;

                  case 2: {
                    list_lensa();
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;

                  case 3: {
                    admin_input_kamera();
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;

                  case 4: {
                    admin_input_lensa();
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;

                  case 5: {
                    admin_hapus_kamera();
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;

                  case 6: {
                    admin_hapus_lensa();
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;

                  /*case 7: {
                    kamera* tail_kamera = lastNode(head_kamera);
                    quickSort(head_kamera, tail_kamera);
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  } break;
                */
                  default:
                    cout << "Pilihan tidak tersedia.\n";
                    cout << "Klik enter untuk melanjutkan\n";
                    cin.get();
                  break;
                }
            } break;

          case 2: {
              char hapus_data_penyewaan;
              display_data_penyewaan();
              cout << "Hapus data penyewaan? (y/n)\n";
              cout << "> ";
              cin >> hapus_data_penyewaan;
              cin.ignore();
            
              if(hapus_data_penyewaan == 'y' || hapus_data_penyewaan == 'Y'){
                  int id_dicari;
                  cout << "Masukkan id member\n";
                  cout << "> "; cin >> id_dicari;
                  hapus_penyewaan(id_dicari);
              }
              cout << "Klik enter untuk melanjutkan\n";
              cin.get();
          } break;

          case 3: {
              int member_choose;
              cout << "\t1. Input member\n";
              cout << "\t2. List member\n";
              cout << "\t3. Hapus member\n";
              cout << "\t > ";
              cin >> member_choose;
              cin.ignore();

              switch (member_choose){
                case 1: {
                  admin_buat_member();
                  cout << "Klik enter untuk melanjutkan\n";
                  cin.get();
                  } break;

                case 2: {
                  admin_lihat_member();
                  cout << "Klik enter untuk melanjutkan\n";
                  cin.get();
                  } break;

                case 3: {
                  admin_hapus_member();
                  cout << "Klik enter untuk melanjutkan\n";
                  cin.get();
                  } break;

                default:
                  cout << "Pilihan tidak tersedia\n";
                  cout << "Klik enter untuk melanjutkan\n";
                  cin.get();
                break;
              }
            } break;

          case 4: {
            int fotografer_choose;
            cout << "\t1. Input fotografer\n";
            cout << "\t2. Hapus fotografer\n";
            cout << "\t3. List fotografer\n"; 
            cout << "\t> ";
            cin >> fotografer_choose;

            switch(fotografer_choose){
              case 1: {
                push(stack);
                cout << "Klik enter untuk melanjutkan\n";
                cin.get();
                clearScreen();
              } break;

              case 2: {
                string name;
                cout << "Masukkan nama fotografer yang ingin dihapus: ";
                cin.ignore();
                getline(cin, name);
                deleteByName(stack, name);

                cout << "Klik enter untuk melanjutkan\n";
                cin.get();

                clearScreen();
              } break;

              case 3: {
                displayStackInteractive(stack);
              } break;

              default:
                  cout << "Pilihan tidak tersedia\n" ;
                  cout << "Klik enter untuk melanjutkan\n";
                  cin.get();
                break;
          } 
        } break;

        case 0: {
          break;

        } break;

        default:
          cout << "Pilihan tidak tersedia\n";
          cout << "Klik enter untuk melanjutkan\n";
          cin.get();
          break;
        }
      } while (choose != 0);
    } else if (pilih == 2) {
      int choose;
      do {
        clearScreen();
        welcome_user();
        cout << "1. Sewa Kamera atau lensa\n";
        cout << "2. Sewa Fotografer\n";
        cout << "3. Rekomendasi spot foto di Surabaya\n";
        cout << "0. Logout\n";
        cout << "> ";

        cin >> choose;
        cin.ignore();
        switch (choose) {
        case 1: {
            int user_choose;
            cout << "\t1. Sewa Kamera\n";
            cout << "\t2. Sewa Lensa\n";
            cout << "\t3. Cari Kamera\n";
            cout << "\t4. Cari Lensa\n";
            cout << "\t> ";
            cin >> user_choose;
            cin.ignore();

            switch(user_choose){
              case 1: {
                user_pesan_kamera();
                cout << "Klik enter untuk melanjutkan\n";
                cin.get();
              } break;

              case 2: {
                user_pesan_lensa();
                cout << "Klik enter untuk melanjutkan\n";
                cin.get();
              } break;

              case 3: {
                user_cari_kamera();
                cout << "Klik enter untuk melanjutkan\n";
                cin.get();
              } break;

              case 4: {
                user_cari_lensa();
                cout << "Klik enter untuk melanjutkan\n";
                cin.get();
              } break;

              default:
              cout << "Pilihan tidak tersedia\n";
              cout << "Klik enter untuk melanjutkan\n";
              cin.get();
              break;
            }
        } break;

        case 2:{
          pesan_fotografer(stack);
          cout << "\nKlik enter untuk melanjutkan\n";
          cin.get();
        } break;

        case 3: {
          clearScreen();
          string destinasi;
          cout << "Spot foto yang recommended di Surabaya\n";
          cout << "1. Kampung Arab , Surabaya Utara\n";
          cout << "2. Kampung Bulak , Surabaya Utara\n";
          cout << "3. Klenteng Sanggar Agung , Surabaya Utara\n";
          cout << "4. Patung Buddha 4 Rupa , Surabaya Utara\n";
          cout << "5. Museum House of Sampoerna, Surabaya Utara\n";
          cout << "6. G-Walk Citraland , Surabaya Barat\n";
          cout << "7. Tugu Pahlawan , Surabaya Pusat\n";
          cout << "8. Museum Pahlawan , Surabaya Pusat\n";
          cout << "9. Jalan Tunjungan, Surabaya Pusat\n";
          cout << "10. Kebun Binatang Surabaya, Surabaya Selatan\n\n";

          cout << "Ketikkan nama tempat terdekat dari posisimu\n";
          cout << "> ";

          getline(cin, destinasi);

          cout << "Mungkin kita bisa merekomendasikan rute spot foto yang dekat dengan destinasimu\n\n";
          BFS(destinasi_foto, destinasi);

          cout << "\nKlik enter untuk melanjutkan\n";
          cin.get();
        } break;

        case 0: {
          break;
        } break;

        default:
          cout << "Pilihan tidak tersedia";
          break;
        }
      } while (choose != 0);
    }
  } while (pilih != 3);
  cout << "Terimakasih! Semoga harimu Senin terus.";
  return 0;

}

// Fungsi untuk menambahkan kamera ke linked list
void admin_input_kamera() {
  clearScreen();
  string merekkamera;
  kamera *newKamera = new kamera;
  cout << "Masukkan merek kamera\t: ";
  getline(cin, merekkamera);
  cout << "Masukkan tipe kamera\t: ";
  getline(cin, newKamera->tipe);
  cout << "Masukkan harga kamera\t: ";
  cin >> newKamera->harga;

  toUpperCase(merekkamera);
  newKamera->merek = merekkamera;

  cin.ignore();
  newKamera->next = newKamera;
  newKamera->prev = newKamera;

  if (head_kamera == nullptr) {
    head_kamera = newKamera;
    tail_kamera = newKamera;
    head_kamera->next = head_kamera;
    head_kamera->prev = head_kamera;
    tail_kamera->next = tail_kamera;
    tail_kamera->prev = tail_kamera;
  } else {
    newKamera->next = head_kamera;
    head_kamera->prev = newKamera;
    head_kamera = newKamera;
    head_kamera->prev = tail_kamera;
    tail_kamera->next = head_kamera;
  }
  cout << "\nKamera berhasil ditambahkan\n";
}

// Fungsi untuk menambahkan lensa ke linked list
void admin_input_lensa() {
  clearScreen();
  string mereklensa;
  lensa *newLensa = new lensa();
  cout << "Masukkan merek lensa\t: ";
  getline(cin, mereklensa);
  cout << "Masukkan tipe lensa\t: ";
  getline(cin, newLensa->tipe);
  cout << "Masukkan harga lensa\t: ";
  cin >> newLensa->harga;

  toUpperCase(mereklensa);
  newLensa->merek = mereklensa;

  cin.ignore();
  newLensa->next = newLensa;
  newLensa->prev = newLensa;

  if (head_lensa == nullptr) {
    head_lensa = newLensa;
    tail_lensa = newLensa;
    head_lensa->next = head_lensa;
    head_lensa->prev = head_lensa;
    tail_lensa->next = tail_lensa;
    tail_lensa->prev = tail_lensa;
  } else {
    newLensa->next = head_lensa;
    head_lensa->prev = newLensa;
    head_lensa = newLensa;
    head_lensa->prev = tail_lensa;
    tail_lensa->next = head_lensa;
  }
  cout << "\nLensa berhasil ditambahkan\n";
}

// Fungsi untuk menghapus kamera dari linked list
void admin_hapus_kamera() {
  clearScreen();
  if (head_kamera == nullptr) {
    cout << "\nTidak ada kamera untuk dihapus\n";
    return;
  }
  string tipe;
  cout << "Masukkan tipe kamera yang ingin dihapus: ";
  getline(cin,tipe);

  bool ketemu = false;
  kamera *temp = head_kamera;
  kamera *prev_node = tail_kamera;
  do{
    if(temp->tipe == tipe){
        ketemu = true;
        if (temp == head_kamera) {
                    head_kamera = temp->next;
                    tail_kamera->next = head_kamera;
                    delete temp;
                } 
                else if (temp == tail_kamera) {
                    tail_kamera = prev_node;
                    tail_kamera->next = head_kamera;
                    delete temp;
                } 
                else {
                    prev_node->next = temp->next;
                    delete temp;
                }
    }
    prev_node = temp;
    temp = temp->next;
  } while (temp != head_kamera);

    if(ketemu == true){
    cout << "Kamera berhasil dihapus dari database\n";
  } else {
    cout << "Lensa dengan tipe " << tipe << " tidak ditemukan\n";
  }
}

// Fungsi untuk menghapus lensa dari linked list
void admin_hapus_lensa() {
  clearScreen();
  if (head_lensa == nullptr) {
    cout << "Tidak ada lensa untuk dihapus\n";
    return;
  }
  string tipe;
  cout << "Masukkan tipe lensa yang ingin dihapus: ";
  getline(cin,tipe);

  bool ketemu = false;
  lensa *temp = head_lensa;
  lensa *prev_node = tail_lensa;
  do{
    if(temp->tipe == tipe){
        ketemu = true;
        if (temp == head_lensa) {
                    head_lensa = temp->next;
                    tail_lensa->next = head_lensa;
                    delete temp;
                } 
                else if (temp == tail_lensa) {
                    tail_lensa = prev_node;
                    tail_lensa->next = head_lensa;
                    delete temp;
                } 
                else {
                    prev_node->next = temp->next;
                    delete temp;
                }
    }
    prev_node = temp;
    temp = temp->next;
  } while (temp != head_lensa);

  if(ketemu == true){
    cout << "Lensa berhasil dihapus dari database\n";
  } else {
    cout << "Lensa dengan tipe " << tipe << " tidak ditemukan\n";
  }
}

// Fungsi untuk menambahkan member ke tree
void admin_buat_member() {
  clearScreen();
  member *newMember = new member();
  cout << "Masukkan nama member\t\t: ";
  getline(cin,newMember->nama_member);

  cout << "Masukkan tgl lahir (dd-mm-yyyy)\t: ";
  getline(cin,newMember->tanggal_lahir);

  cout << "Masukkan no telp\t\t: ";
  cin >> newMember->no_telp;
  newMember->left = nullptr;
  newMember->right = nullptr;

  newMember->id = buat_id(
      newMember->nama_member, newMember->tanggal_lahir, newMember->no_telp);

  cin.ignore();
  if (root == nullptr) {
    root = newMember;
  } else {
    member *current = root;
    member *parent = nullptr;
    while (current != nullptr) {
      parent = current;
      if (newMember->nama_member < current->nama_member) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    if (newMember->nama_member < parent->nama_member) {
      parent->left = newMember;
    } else {
      parent->right = newMember;
    }
  }
  cout << "\nMember berhasil dibuat\n";
  cout << "\nId Member\t: " << newMember->id << endl;
  cout << "Nama\t\t: " << newMember->nama_member << endl;
  cout << "Tanggal Lahir\t: " << newMember->tanggal_lahir << endl;
  cout << "No Telp\t\t: " << newMember->no_telp << endl;
}

// Fungsi untuk melihat daftar member (inorder traversal)
void inorder(member *root) {
  if (root != nullptr) {
    inorder(root->left);
    cout << "\nID Member\t: " << root->id << endl;
    cout << "Nama\t\t: " << root->nama_member << endl;
    cout << "Tanggal Lahir\t: " << root->tanggal_lahir << endl;
    cout << "No Telp\t\t: " << root->no_telp << endl;
    inorder(root->right);
  }
}

// Fungsi untuk melihat daftar member
void admin_lihat_member() {
  if (root == nullptr) {
    cout << "Tidak ada member\n";
    return;
  } else {
    inorder(root);
  }
}

//Fungsi untuk menghapus member
void admin_hapus_member() {
  if (root == nullptr) {
    return;
  }

  string memberName;
  cout << "Masukkan nama member yang ingin dihapus: ";
  cin >> memberName;

  member *current = root;
  member *parent = nullptr;
  while (current != nullptr && current->nama_member != memberName) {
    parent = current;
    if (memberName < current->nama_member) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  if (current == nullptr) {
    return;
  }

  if (current->left == nullptr) {
    member *rightChild = current->right;
    if (current == root) {
      root = rightChild;
    } else {
      if (parent->left == current) {
        parent->left = rightChild;
      } else {
        parent->right = rightChild;
      }
    }
    if (rightChild != nullptr) {
      rightChild->parent = parent;
    }
  } else {
    member *leftChild = current->left;
    member *successor = leftChild;
    while (successor->right != nullptr) {
      successor = successor->right;
    }
    if (successor->left != nullptr) {
      successor->left->parent = successor;
    }
    successor->left = leftChild;
    successor->right = current->right;
    if (current == root) {
      root = successor;
    } else {
      if (parent->left == current) {
        parent->left = successor;
      } else {
        parent->right = successor;
      }
    }
    successor->parent = parent;
    if (current->right != nullptr) {
      current->right->parent = successor;
    }
  }

  delete current;

  cout << "Member berhasil di hapus!\n";
  cout << "Enter untuk melanjutkan";
  cin.get();
}

void user_buat_member_untuk_kamera(kamera *kamera_yang_disewa) {
  clearScreen();
  member *newMember = new member();
  cout << "Masukkan nama member\t\t: ";
  getline(cin,newMember->nama_member);

  cout << "Masukkan tgl lahir (dd-mm-yyyy)\t: ";
  getline(cin,newMember->tanggal_lahir);

  cout << "Masukkan no telp\t\t: ";
  cin >> newMember->no_telp;
  newMember->left = nullptr;
  newMember->right = nullptr;

  newMember->id = buat_id(
      newMember->nama_member, newMember->tanggal_lahir, newMember->no_telp);

  cin.ignore();
  if (root == nullptr) {
    root = newMember;
  } else {
    member *current = root;
    member *parent = nullptr;
    while (current != nullptr) {
      parent = current;
      if (newMember->nama_member < current->nama_member) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    if (newMember->nama_member < parent->nama_member) {
      parent->left = newMember;
    } else {
      parent->right = newMember;
    }
  }
  cout << "\nMember berhasil dibuat\n";
  cout << "\nID Member\t: " << newMember->id << endl;
  cout << "Nama\t\t: " << newMember->nama_member << endl;
  cout << "Tanggal Lahir\t: " << newMember->tanggal_lahir << endl;
  cout << "No Telp\t\t: " << newMember->no_telp << endl;

  int lama_sewa;
  cout << "\nIngin sewa kamera berapa hari?\n";  
  cin >> lama_sewa;

  int harga_total = kamera_yang_disewa->harga * lama_sewa;

  sewa_kamera(newMember->id,newMember->nama_member,kamera_yang_disewa,lama_sewa,harga_total);
}

void user_buat_member_untuk_lensa(lensa *lensa_yang_disewakan) {
  clearScreen();
  member *newMember = new member();
  cout << "Masukkan nama member\t\t: ";
  getline(cin,newMember->nama_member);

  cout << "Masukkan tgl lahir (dd-mm-yyyy)\t: ";
  getline(cin,newMember->tanggal_lahir);

  cout << "Masukkan no telp\t\t: ";
  cin >> newMember->no_telp;
  newMember->left = nullptr;
  newMember->right = nullptr;

  newMember->id = buat_id(
      newMember->nama_member, newMember->tanggal_lahir, newMember->no_telp);

  cin.ignore();
  if (root == nullptr) {
    root = newMember;
  } else {
    member *current = root;
    member *parent = nullptr;
    while (current != nullptr) {
      parent = current;
      if (newMember->nama_member < current->nama_member) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    if (newMember->nama_member < parent->nama_member) {
      parent->left = newMember;
    } else {
      parent->right = newMember;
    }
  }
  cout << "\nMember berhasil dibuat\n";
  cout << "\nID Member\t: " << newMember->id << endl;
  cout << "Nama\t\t: " << newMember->nama_member << endl;
  cout << "Tanggal Lahir\t: " << newMember->tanggal_lahir << endl;
  cout << "No Telp\t\t: " << newMember->no_telp << endl;

  int lama_sewa;
  cout << "\n Ingin sewa lensa berapa hari?\n";  
  cin >> lama_sewa;

  int harga_total = lensa_yang_disewakan->harga * lama_sewa;

  sewa_lensa(newMember->id,newMember->nama_member,lensa_yang_disewakan,lama_sewa,harga_total);
}

// Fungsi untuk pengguna memesan kamera
void user_pesan_kamera() {
  if (head_kamera == nullptr) {
    cout << "Tidak ada kamera yang tersedia untuk dipesan\n";
    return;
  }
  kamera *temp = head_kamera;
  int pilih;
  do {
    clearScreen();
    welcome_user();
    cout << "Merek Kamera\t\t: " << temp->merek << endl;
    cout << "Tipe Kamera\t\t: " << temp->tipe << endl;
    cout << "Harga Sewa (per hari)\t: " << temp->harga << endl;
    cout << "1. Lihat Kamera Selanjutnya\n";
    cout << "2. Lihat Kamera Sebelumnya\n";
    cout << "3. Pesan Kamera\n";
    cout << "0. Exit\n> ";
    cin >> pilih;
    cin.ignore();
    switch (pilih) {
    case 1: {
      temp = temp->next;
    } break;
    case 2: {
      temp = temp->prev;
    } break;
    case 3:{
      user_buat_member_untuk_kamera(temp);
      cout << "Klik enter untuk melanjutkan\n";
      cin.get();
    } break;
    case 0:
      break;
    default:
      cout << "Pilihan Tidak Valid!";
      break;
    }
  } while (pilih != 0);
}

// Fungsi untuk pengguna memesan lensa
void user_pesan_lensa() {
  if (head_lensa == nullptr) {
    cout << "Tidak ada lensa yang tersedia untuk dipesan\n";
    return;
  }
  lensa *temp = head_lensa;
  int pilih;
  do {
    clearScreen();
    welcome_user();
    cout << "Merek Lensa\t\t: " << temp->merek << endl;
    cout << "Tipe Lensa\t\t: " << temp->tipe << endl;
    cout << "Harga Sewa (per hari)\t: " << temp->harga << endl;
    cout << "1. Lihat Lensa Selanjutnya\n";
    cout << "2. Lihat Lensa Sebelumnya\n";
    cout << "3. Pesan Lensa\n";
    cout << "0. Exit\n> ";
    cin >> pilih;
    cin.ignore();
    switch (pilih) {
    case 1: {
      temp = temp->next;
    } break;
    case 2: {
      temp = temp->prev;
    } break;
    case 3: {
      user_buat_member_untuk_lensa(temp);
      cout << "Klik enter untuk melanjutkan\n";
      cin.get();
    } break;

    case 0:
      break;
    default:
      cout << "Pilihan Tidak Valid!";
      break;
    }
  } while (pilih != 0);
}

// Fungsi untuk mencari kamera
void user_cari_kamera() {
  if (head_kamera == nullptr) {
    cout << "Tidak ada kamera yang tersedia\n";
    return;
  } else {
    clearScreen();
    string tipe_yang_dicari;
    cout << "Masukkan merek kamera yang ingin dicari: ";
    getline(cin,tipe_yang_dicari);

    toUpperCase(tipe_yang_dicari);

    kamera *temp = head_kamera;
    while (temp->next != head_kamera && temp->tipe != tipe_yang_dicari) {
      temp = temp->next;
    }

    if (temp->next == head_kamera) {
      cout << "\nKamera dengan merek " << tipe_yang_dicari << " tidak ditemukan\n";
    } else {
      cout << "\nKamera ditemukan:\n" << temp->merek << " " << temp->tipe << " , dengan harga sewa per hari "
           << temp->harga << endl << endl;
    }
  }
}

// Fungsi untuk mencari lensa
void user_cari_lensa() {
  if (head_lensa == nullptr) {
    cout << "Tidak ada lensa yang tersedia\n";
    return;
  } else {
    clearScreen();
    string tipe_yang_dicari;
    cout << "Masukkan tipe lensa yang ingin dicari: ";
    getline(cin,tipe_yang_dicari);

    toUpperCase(tipe_yang_dicari);

    lensa *temp = head_lensa;
    while (temp->next != head_lensa && temp->tipe != tipe_yang_dicari) {
      temp = temp->next;
    }

    if (temp->next == head_lensa) {
      cout << "\nLensa dengan tipe " << tipe_yang_dicari << " tidak ditemukan\n";
    } else {
      cout << "\nLensa ditemukan:\n" << temp->merek << " " << temp->tipe << " , dengan harga sewa per hari "
           << temp->harga << endl << endl;
    }
  }
}

// Fungsi untuk menampilkan daftar kamera
void list_kamera() {
  if (head_kamera == nullptr) {
    cout << "\nTidak ada kamera yang tersedia\n\n";
    return;
  }
  int pilih;
  kamera *temp = head_kamera;
  do {
    clearScreen();
    welcome();

    cout << "Merek Kamera\t\t: " << temp->merek << endl;
    cout << "Tipe Kamera\t\t: " << temp->tipe << endl;
    cout << "Harga Sewa (per hari)\t: " << temp->harga << endl;
    cout << "1. Lihat Kamera Selanjutnya\n";
    cout << "2. Lihat Kamera Sebelumnya\n";
    cout << "0. Exit\n> ";
    cin >> pilih;

    switch (pilih) {
    case 1: {
      temp = temp->next;
    } break;
    case 2: {
      temp = temp->prev;
    } break;
    case 0:
      break;
    default:
      cout << "Pilihan Tidak Valid!";
      break;
    }
    cout << "3. Kembali\n";
  } while (pilih != 0);
}

// Fungsi untuk menampilkan daftar lensa
void list_lensa() {
  if (head_lensa == nullptr) {
    cout << "\nTidak ada lensa yang tersedia\n\n";
    return;
  }
  int pilih;
  lensa *temp = head_lensa;
  do {
    clearScreen();
    welcome();
    cout << "Merek Lensa\t\t: " << temp->merek << endl;
    cout << "Tipe Lensa\t\t: " << temp->tipe << endl;
    cout << "Harga Sewa (per hari)\t: " << temp->harga << endl;
    cout << "1. Lihat Lensa Selanjutnya\n";
    cout << "2. Lihat Lensa Sebelumnya\n";
    cout << "0. Exit\n> ";
    cin >> pilih;
    switch (pilih) {
    case 1:{
      temp = temp->next;
    } break;
    case 2:{
      temp = temp->prev;
    } break;
    case 0:
      break;
    default:
      cout << "Pilihan Tidak Valid!";
      break;
    }
  } while (pilih != 0);
}

int buat_id(string nama, string tanggal_lahir, string no_telp) {
  char kata_nama_depan = nama[0];
  char kata_tanggal_lahir_depan = tanggal_lahir[0];

  int nilai_kata_nama_depan = kata_nama_depan - 'A' + 1;
  if (nilai_kata_nama_depan < 10) {
    nilai_kata_nama_depan = '0' + nilai_kata_nama_depan;
  }

  int nilai_kata_tanggal_lahir_depan = kata_tanggal_lahir_depan - 'A' + 1;
  if (nilai_kata_tanggal_lahir_depan < 10) {
    nilai_kata_tanggal_lahir_depan = '0' + nilai_kata_tanggal_lahir_depan;
  }

  int dua_nilai_terakhir_no_telp = stoi(no_telp.substr(no_telp.length() - 2));

  int id_fix = nilai_kata_nama_depan * 10000 + nilai_kata_tanggal_lahir_depan * 100 + dua_nilai_terakhir_no_telp;

  return id_fix;
}

void input_awal_kamera(string brand, string type, int price){
  kamera *newKamera = new kamera();

  toUpperCase(brand);

  newKamera->merek = brand;
  newKamera->tipe = type;
  newKamera->harga = price;

  newKamera->next = newKamera;
  newKamera->prev = newKamera;

  if (head_kamera == nullptr) {
    head_kamera = newKamera;
    tail_kamera = newKamera;
    head_kamera->next = head_kamera;
    head_kamera->prev = head_kamera;
    tail_kamera->next = tail_kamera;
    tail_kamera->prev = tail_kamera;
  } else {
    newKamera->next = head_kamera;
    head_kamera->prev = newKamera;
    head_kamera = newKamera;
    head_kamera->prev = tail_kamera;
    tail_kamera->next = head_kamera;
  }
}

void input_awal_lensa(string brand, string type, int price){
  lensa *newLensa = new lensa();

  toUpperCase(brand);

  newLensa->merek = brand;
  newLensa->tipe = type;
  newLensa->harga = price;

  newLensa->next = newLensa;
  newLensa->prev = newLensa;

  if (head_lensa == nullptr) {
    head_lensa = newLensa;
    tail_lensa = newLensa;
    head_lensa->next = head_lensa;
    head_lensa->prev = head_lensa;
    tail_lensa->next = tail_lensa;
    tail_lensa->prev = tail_lensa;
  } else {
    newLensa->next = head_lensa;
    head_lensa->prev = newLensa;
    head_lensa = newLensa;
    head_lensa->prev = tail_lensa;
    tail_lensa->next = head_lensa;
  }
}
