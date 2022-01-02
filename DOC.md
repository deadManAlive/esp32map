# Monitor Jaringan Wi-Fi Jarak Jauh dengan ESP-32

Muhammad Rafky Alfarrakhan.s (190 638 3974)

## Permasalahan

Internet, sejak dekade yang lalu dan semakin pesat belakangan berkat pandemi yang memaksa komunikasi jarak jauh, semankin erat dengan kehidupan manusia sehingga mengatakan dalam beberapa tahun internet akan menjadi kebutuhan dasar adalah hal yang tepat. Wi-Fi sebagai salah satu bentuk koneksi internet juga semakin dibutuhkan dan menjamur dimana-mana, meskipun tidak sepesat pertumbuhan user internet (Internet provider terbesar di Indonesia, IndiHome, memiliki [kurang dari 10 juta pelanggan pada 2020](https://industri.kontan.co.id/news/indihome-bidik-jumlah-pelanggan-hingga-akhir-2020-capai-85-juta). Sementara itu, [pengguna *smartphone* berada pada angka 60% populasi pada 2019](https://databoks.katadata.co.id/datapublish/2020/09/15/pengguna-smartphone-diperkirakan-mencapai-89-populasi-pada-2025)). Oleh karena itu, terdapat potensi yang tinggi pengguna ilegal ataupun tidak bertanggung jawab pada sebuah jaringan Wi-Fi, terutama pada jaringan Wi-Fi rumahan dengan fitur keamanan lebih rendah. Terinspirasi dari **[fingbox oleh fing](https://www.fing.com/products/fingbox)**, proyek ini bertujuan untuk menciptakan sebuah perangkat yang dapat memonitor perangkat-perangkat yang terkoneksi pada sebuah jaringan Wi-Fi, setidaknya apakah terjadi inisiasi koneksi yang tidak diharapkan.

Proyek ini akan memanfaatkan fitur-fitur ESP-32 sebagai berikut:

1.   ***Web Server/External API Resources***: Monitor dapat dilakukan secara jauh dengan notifikasi email melalui API webhooks dari IFTTT.
2.   ***Non-Volatile Storage***: Menyimpan log koneksi dalam penyimpanan non volatile yang dapat diperiksa melewati 1 siklus power. Proyek ini menggunakan SPIFFS.
3.   ***Deepsleep/Watchdog***: Karena koneksi dan *scan* jaringan WiFi cukup membutuhkan daya, maka fitur deepsleep digunakan untuk mengurangi konsumsi daya dengan mematikan mesin dalam tiap sekali siklus *scan* untuk 5 menit.

## Solusi Permasalahan

Hal utama yang penulis perhatikan dalam penyusunan perangkat ini ialah minimalisme, dimana perangkat ESP32 telah memiliki banyak fitur yang dapat digunakan dalam proyek ini. Sebagai inti dari network scanning, penulis menggunakan library ping dan kemudian melakukan parsing alamat ip dan submask untuk mendapatkan range dari host-host yang harus discan. Dengan meng-scan satu per satu host, maka host yang memberikan respons ping (ICMP) dapat dikatakan sebagai host aktif dan terkoneksi pada jaringan. Untuk itu, penulis menggunakan penyimpanan non volatile untuk mencatat IP address dan dapat membandingkan untuk melihat apakah ada alamat baru pada siklus selanjutnya, dengan menggunakan API webhooks dari IFTTT, maka notifikasi berupa email dapat dikirimkan ke admin.

## Metodologi

Karena penulis berusaha membuat perangkat seminimal mungkin, penulis dapat mengeliminasi kebutuhan rangkaian cukup menjadi board ESP32 saja, diluar router sendiri dan catu daya. Penulis melihat bahwa software yang dapat lebih fleksibel dari pada hardware memungkinkan kapabilitas proyek ini dapat jauh dimaksimalkan dengan mengoptimasi algoritmanya. 

1.   external API: Agar monitor dapat dilakukan secara jarak jauh tanpa harus berada pada jaringan yang dimonitor, penulis menggunakan API webhook dari IFTTT, dimana dengan memberikan POST http berupa data yang akan diberikan ke suatu url yang telah diatur, maka event yang berujung pada pengiriman email ke alamat yang telah ditentukan sebelumnya terjadi dengan membawa data yang dikirm dari ESP32.
2.   NV storage: Hal yang penting dilakukan oleh perangkat ini ialah mendeteksi adanya perangkat baru dalam jaringan, dan hal tersebut dapat dicapai dengan menyimpan data IP yang telah dicatat sebelumnya dalam penyimpanan NV, dalam hal ini SPIFFS. Penulis memilih SPIFFS karena penyimpanan tersebut berupa built-in storage dari ESP32 namun dapat menyimpan dalam format yang lebih fleksibel dari preferences. Data disimpan dalam bentuk `.txt` dengan tiap IP addres disimpan perbaris dalam bentuk integer, untuk memudahkan konversi saat proses pembacaan nantinya.
3.   deepsleep: Berdasarkan pengalaman penulis yang menemukan bahwa perangkat lebih cepat panas ketika berurusan dengan jaringan WiFi, maka fitur deepsleep dapat membantu dalam mengurangi konsumsi daya akibat hal tersebut. Pada proyek ini, deepsleep akan terjadi untuk setiap kali siklus scanning berakhir, selama 5 menit.

## Implementasi

Implementasi murni berupa kode-kode dapat dilihat pada tautan berikut:

