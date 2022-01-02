# Monitor Jaringan Wi-Fi Jarak Jauh dengan ESP-32

Muhammad Rafky Alfarrakhan.s (190 638 3974)

## Permasalahan

Internet, sejak dekade yang lalu dan semakin pesat belakangan berkat pandemi yang memaksa komunikasi jarak jauh, semankin erat dengan kehidupan manusia sehingga mengatakan dalam beberapa tahun internet akan menjadi kebutuhan dasar adalah hal yang tepat. Wi-Fi sebagai salah satu bentuk koneksi internet juga semakin dibutuhkan dan menjamur dimana-mana, meskipun tidak sepesat pertumbuhan user internet (Internet provider terbesar di Indonesia, IndiHome, memiliki [kurang dari 10 juta pelanggan pada 2020](https://industri.kontan.co.id/news/indihome-bidik-jumlah-pelanggan-hingga-akhir-2020-capai-85-juta). Sementara itu, [pengguna *smartphone* berada pada angka 60% populasi pada 2019](https://databoks.katadata.co.id/datapublish/2020/09/15/pengguna-smartphone-diperkirakan-mencapai-89-populasi-pada-2025)). Oleh karena itu, terdapat potensi yang tinggi pengguna ilegal ataupun tidak bertanggung jawab pada sebuah jaringan Wi-Fi, terutama pada jaringan Wi-Fi rumahan dengan fitur keamanan lebih rendah. Terinspirasi dari **[fingbox oleh fing](https://www.fing.com/products/fingbox)**, proyek ini bertujuan untuk menciptakan sebuah perangkat yang dapat memonitor perangkat-perangkat yang terkoneksi pada sebuah jaringan Wi-Fi, setidaknya apakah terjadi inisiasi koneksi yang tidak diharapkan.

Proyek ini akan memanfaatkan fitur-fitur ESP-32 sebagai berikut:

1.   ***Web Server/External API Resources***: Monitor dapat dilakukan secara jarak jauh dengan mengirim data MQTT.
2.   ***Non-Volatile Storage***: Menyimpan log koneksi dalam penyimpanan non volatile yang dapat diperiksa melewati 1 siklus power.
3.   

