# **LAPORAN STRUKTUR DATA DAN ALGORITMA(A) - FAUZIAN RAMADHAN(5024241080)**
Particle collision dengan implementasi Bruteforce & Quadtree

# **PENDAHULUAN**
Simulasi ini memperlihatkan sebuah **particle** yang bertabrakan atau dibilangnya **collision** yang outputnya memperlihatkan bola bola yang mantul satu sama lain. Simulasi ini fokus ke metode **Quadtree** dan **Brute Force**, kedua implementasi ini dapat memperlihatkan perbedaan pencarian collision pada bola. Jika menggunakan mode brute force output akan lebih lambat dibandingankan dengan menggunakan quadtree diakrenakan brute force mendetect suatu collision itu pada semua bola jadi kompleksitas waktu **O(n^2)** lama sedangkan quad tree mengecek bola di daerah yang terjadi collision kompleksitas waktunya bisa jadi **(log N)** jadi sangat cepat. \

# **TUJUAN**
- Mengetahui apa saja perbedaan algoritma brute force dan quadtree
- Mengetahui apa saja itu kegunaan quadtree dalam simulasi particle collision

# **Fitur Utama**

- Bola bergerak ke seluruh arah secara otomatis
- Collision bola yang membuat bola pantul dengan satu sama lain dan pantul terhadap window
- Dua mode algoritma yang bisa dignti yaitu **Quad Tree** dan **Brute Force**
- Quadtree window yang menampilkan pembagian ruang dari quadtree
- tombol untuk mengguba quad tree 1 untuk brute force, 2 untuk quadtree
- tombol untuk menambah bola dan kecepatan
- Ui yang memperlihatkan Fps, mode yang digunakan, jumlah bola, kecepatan, dan kontrol

# **Keybinds**

| Tombol      | Fungsi                        |
| ----------- | ----------------------------- |
| **1**       | Mode Brute Force              |
| **2**       | Mode QuadTree                 |
| **↑ Up**    | Tambah 50 bola                |
| **↓ Down**  | Hapus 50 bola                 |
| **→ Right** | Tingkatkan kecepatan partikel |
| **← Left**  | Kurangi kecepatan partikel    |
| **Esc / X** | Keluar                        |



# **Penjelasan Code**
Note :File gabungan ada di folder Project-Collision yang bernama main.cpp sudah tergabung algoritma Bruteforce dan Quadtree 

###  Collision particle
    void collideSwap(Ball& A, Ball& B) {
    float dx = B.x - A.x, dy = B.y - A.y;
    float dist2 = dx*dx + dy*dy;
    float minDist = 2 * RADIUS;
    
    if (dist2 >= minDist*minDist || dist2 == 0) return;

    float d = std::sqrt(dist2);
    sf::Vector2f n = { dx/d, dy/d };

    sf::Vector2f rv = B.vel - A.vel;
    float rel = rv.x*n.x + rv.y*n.y;
    if (rel > 0) return;

    float j = -rel;
    A.vel -= j*n;
    B.vel += j*n;
    }

### Brute Force Collision Check (O(N²))

    for (int i = 0; i < n; ++i)
      for (int j = i + 1; j < n; ++j)
        collideSwap(balls[i], balls[j]);
Kelemahan:
- Setiap bola dibandingkan dengan semua bola → melambat drastis di atas ~2000 bola.


### QuadTree Insertion & Query (O(N log N))
    QuadTree qt({0, 0, WINDOW_W, WINDOW_H}, 6);
    for (auto &b : balls) qt.insert(&b);

    for (auto &b : balls) {
    sf::FloatRect range = b.shape.getGlobalBounds();
    std::vector<Ball*> found;
    qt.query(range, found);

        for (auto *o : found)
          if (&b < o)
            collideSwap(b, *o);
    }
Keunggulan:
- Hanya bola yang berada di region yang sama yang dicek → sangat cepat.

QuadTree Visualization

Berikut gambaran subdivisi pertama QuadTree, ketika ruang utama terbagi menjadi 4 region:

### Visual Example



```
+-------+-------+       
|   NW   |   NE   |  
+-------+-------+       
|   SW   |   SE   |
+-------+-------+
```





- **NW (North-West)**  → kiri atas  
- **NE (North-East)**  → kanan atas  
- **SW (South-West)**  → kiri bawah  
- **SE (South-East)**  → kanan bawah  

Di dalam programmu, subdivisi lebih lanjut akan muncul bila suatu region memiliki
lebih banyak bola daripada `capacity` (default: 6). Setiap subdivisi akan
membentuk grid hijau yang semakin kecil, sesuai kedalaman QuadTree.




# **Perbandingan Brute Force vs QuadTree**
| Aspek                 | Brute Force             | QuadTree                   |
| --------------------- | ----------------------- | -------------------------- |
| Kompleksitas          | O(N²)                   | O(N log N)                 |
| Kecepatan (5000 bola) | ❌ Sangat lambat         | ✔ Sangat cepat             |
| Struktur Data         | Tidak ada               | Pohon 4 ruang              |
| Deteksi Tetangga      | Semua bola dibandingkan | Hanya bola dalam subdivisi |
| Stabilitas FPS        | Buruk                   | Bagus                      |
| Visual                | Tidak ada               | Grid QuadTree hijau        |

https://github.com/user-attachments/assets/204513ca-642c-4781-b32f-2176dfecbd06

# **Kesimpulan**
Pada simulasi berhasi mengimplementasikan brute force dan quad tree yang menunjukkan keefektifannya quad tree untuk melacak suatu collision dengan sangan cepat **(log N)** dengan cara pembagian grid pada daerah sebanyak 4 untuk jika terjadi collision di suatu daerah, daerah itu saja yang di cek tidak semua.
Dibandingkan dengan brute force yang mendetect semua bola yang bertabrakan bikin waktu jadi tidak efisien dan lambat **O(n^2)** dan alhasil pengedropan fps karena semua bola itu di cek satu persatu.
