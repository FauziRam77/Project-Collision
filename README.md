# **Quadtree/BruteForce Collision**

# **PENDAHULUAN**
Simulasi ini memperlihatkan sebuah **particle** yang bertabrakan atau dibilangnya **collision** yang outputnya memperlihatkan bola bola yang mantul satu sama lain. Simulasi ini fokus ke metode **Quadtree** dan **Brute Force**, kedua implementasi ini dapat memperlihatkan perbedaan pencarian collision pada bola. Jika menggunakan mode brute force output akan lebih lambat dibandingankan dengan menggunakan quadtree diakrenakan brute force mendetect suatu collision itu pada semua bola jadi kompleksitas waktu **O(n^2)** lama sedangkan quad tree mengecek bola di daerah yang terjadi collision kompleksitas waktunya bisa jadi **(log N)** jadi sangat cepat. \

# **TUJUAN**
-Mengetahui apa saja perbedaan algoritma brute force dan quadtree
-mengetahui apa saja itu kegunaan quadtree dalam simulasi particle collision

# **Fitur Utama**

- Bola bergerak ke seluruh arah secara otomatis
- Collision bola yang membuat bola pantul dengan satu sama lain dan pantul terhadap window
- Dua mode algoritma yang bisa dignti yaitu **Quad Tree** dan **Brute Force**
- Quadtree window yang menampilkan pembagian ruang dari quadtree
- tombol untuk mengguba quad tree 1 untuk brute force, 2 untuk quadtree
- tombol untuk menambah bola dan kecepatan
- Ui yang memperlihatkan Fps, mode yang digunakan, jumlah bola, kecepatan, dan kontrol

# **Kesimpulan**
Pada simulasi berhasi mengimplementasikan brute force dan quad tree yang menunjukkan keefektifannya quad tree untuk melacak suatu collision dengan sangan cepat **(log N)** dengan cara pembagian grid pada daerah sebanyak 4 untuk jika terjadi collision di suatu daerah, daerah itu saja yang di cek tidak semua.
Dibandingkan dengan brute force yang mendetect semua bola yang bertabrakan bikin waktu jadi tidak efisien dan lambat **O(n^2)** dan alhasil pengedropan fps karena semua bola itu di cek satu persatu.
