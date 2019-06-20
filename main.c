#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <wchar.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//Ayarların konumları
enum ayar{TakimSayisi,GalibiyetPuan,BeraberlikPuan,MaglubiyetPuan};
enum puan_tablosu{PuanSirasina, TakimlarinAlfabetikSirasina, TakmaIsimSirasina};

struct TAKIM{
    char takmaAd;
    int puan;
    int oynadigiMacSayisi;
    int icSahadaMacSayisi;
    int galibiyet;
    int beraberlik;
    int maglubiyet;
    int atilanGol;
    int yedigiGol;
    char* isim;
    char* kendiSahasindaMacYaptigiTakimlar;
};
typedef struct TAKIM* Takim;


struct TABLO{//puan durumu tablosunda kullanılma üzerer lite
    int sira;
    Takim takim;
};
typedef struct TABLO* Tablo;


Takim *takimlar;//Takımları tutan degisken
int ayarlar[4];//0: takim sayisi. 1:galibiyet puan. 2:beraberlik puan. 3:mağlubiyet puan.


void turkceKarakterDuzelt();//Konsolda tükçe karakterler duzgun gorunmesini sağla
char* dosyayiOku(char* );// belirtilen dosyayı oku içerigi char* tipinde dondur
void ayarlariAl();  //Ayarları dosyadan oku ve diziye koy
void takimlariOlustur();//Takımları dosyadan oku ve listeyi olustur.
Takim takimOlustur(char ,char* );//Takma adi ve ismi belirtilen takımı olustur
void takimlariYokEt();//Takimlar listesini yok et. Heapta gereksiz yer kaplama
void klavyedenMacGir();//Klavyeden giris yapılmasını iste
void macIsle(char evSahibiTakim,int evSahibiGol,char misafirTakim,int misafirGol);//girilen maci skorlara isle
void dosyadanMacOku(); //Macların bolundugu dosyadan macları oku ve işle
Tablo* puanDurumuTablosu(int secenek);//Belirtilen seceneğe göre tablo olusturur.
Tablo* takimListesiTablosunuOlustur();//Takma ad sirasina gore tabloyu olustur.
Tablo tabloOlustur(int sira,Takim takim);//Tablo structundan eleman olustur
void tabloyuEkranaYaz(Tablo* t);//Verilen Tabloyu ekrana yaz
void puanSirasinaGoreSirala(Tablo* t);//verilen tabloyu puan sırasına gore sirala averajı goz onunde bulundur
void takimlarinAlfabetikSirasinaGoreSirala(Tablo* t);//Verilen tabloyu takim ismine gore siralar.
bool kucukMu(char* ilk, char* ikinci);//ilk metin ile ikinci metni karsilastirir ilk metin daha kucukse true doner.
void dongu();//istemleri tekrar tekrar yapabilmek icin gerekli dongu
void takimIsimleriniBuyukYap(Tablo* t);//Tablodaki takım isimlerini buyuk harf yapar
void tabloyuDosyayaYaz(Tablo* t);//verilen tabloyu dosyaya yazar

int main()
{
    turkceKarakterDuzelt();
    ayarlariAl();
    takimlariOlustur();
    dongu();
    return 0;
}

void dongu(){
    bool cikis = false;
    Tablo* tablo = puanDurumuTablosu(TakmaIsimSirasina);
    while(!cikis){
        int secim;
        printf("\nKlavyeden Maç Gir:1\nDosyadan Maç Oku:2\nTabloyu puana göre sırala:3\n");
        printf("Tabloyu takma isime göre sırala:4\nTabloyu takım isimlerine göre sırala:5\n");
        printf("Tablodaki takim isimlerini buyuk yap:6\nTabloyu ekrana yaz:7\nTabloyu dosyaya yaz:8\nÇıkış:9\n");
        scanf("%d",&secim);
        switch( secim) {
            case 9: cikis=true; break;
            case 8: tabloyuDosyayaYaz(tablo);break;
            case 7: tabloyuEkranaYaz(tablo); break;
            case 6: takimIsimleriniBuyukYap(tablo);break;
            case 5: tablo = puanDurumuTablosu(TakimlarinAlfabetikSirasina); break;
            case 4: tablo = puanDurumuTablosu(TakmaIsimSirasina); break;
            case 3: tablo = puanDurumuTablosu(PuanSirasina); break;
            case 2: dosyadanMacOku(); break;
            case 1: klavyedenMacGir();break;
            default:
                printf("HATA: Bilinmeyen bir değer girdiniz!\n");
        }
    }
    takimlariYokEt();
}

void tabloyuDosyayaYaz(Tablo* t){
    char* dosyaAdi = "puanTablosu.txt";
    FILE *fp = fopen(dosyaAdi, "w");
    fprintf(fp,"\n%-2s %-2s %-15.15s\t%3s %3s %3s %3s %3s %3s %3s %4s\n","S","TA","Takim Ismi","OM", "G", "B", "M", "AG","YG","AV","Puan");
    fprintf(fp,"------------------------------------------------------------\n");
    for(int i = 0; i < ayarlar[TakimSayisi]; i++){
        char takmaAd = t[i]->takim->takmaAd;
        char* isim = t[i]->takim->isim;

        int om = t[i]->takim->oynadigiMacSayisi;
        int g = t[i]->takim->galibiyet;
        int b = t[i]->takim->beraberlik;
        int m = t[i]->takim->maglubiyet;
        int ag = t[i]->takim->atilanGol;
        int yg = t[i]->takim->yedigiGol;
        int averaj = t[i]->takim->atilanGol-t[i]->takim->yedigiGol;
        int puan = t[i]->takim->puan;
        fprintf(fp,"%-2d %-2c %-15.15s\t%3d %3d %3d %3d %3d %3d %3d %3d\n", t[i]->sira, takmaAd, isim, om, g, b, m , ag, yg, averaj, puan);
        fprintf(fp,"------------------------------------------------------------\n");
    }
    fclose(fp);
    printf("%s isimli dosyaya başarı ile yazıldı.\n",dosyaAdi);
}

void takimIsimleriniBuyukYap(Tablo* t){
    int n = ayarlar[TakimSayisi];
    for(int i = 0; i < n; i++){
        char* isim = t[i]->takim->isim;
        int uzunluk = strlen(isim);
        for(int j=0; j<uzunluk; j++){
            //Türkçe karakterleri duzeltme
            if(isim[j]==-61){//Ç
                if(isim[j+1]==-89)
                isim[j+1] = -121;
            }
            if(isim[j]==-60){
                if(isim[j+1] == -97){//Ğ
                    isim[j+1] = -98;
                }else if (isim[j+1] == -79){//I
                    isim[j] = 73;
                    isim[j+1] = 30;
                }
            }
            if(isim[j]==-59){
                if(isim[j+1]==-97){//ş
                    isim[j+1]=-98;
                }
            }
            if(isim[j]==-61){
                if(isim[j+1]==-74){//ö
                    isim[j+1]=-106;
                }else if (isim[j+1]==-68){//ü
                    isim[j+1]=-100;
                }
            }
            isim[j] = toupper(isim[j]);
        }
    }
}

bool kucukMu(char* ilk, char* ikinci){
    int ilkU = strlen(ilk);
    int ikinciU = strlen(ikinci);
    int min = MIN(ilkU,ikinciU);
    for(int i = 0; i < min; i++){//Kucuk olanın boyu kadar don
        if(ilk[i] < ikinci[i]) return true;//ilk metin daha kucuk
        else if(ikinci[i] < ilk[i]) return false;//ikinci daha kucuk
    }
    if(ilkU < ikinciU) return true;
    return false;//esitlerse false dondur ve islem yapılmasın
}

void takimlarinAlfabetikSirasinaGoreSirala(Tablo* t){
    int n = ayarlar[TakimSayisi];
    for(int i = 0; i < n-1; i++){//Bubble short
        for(int j = 0; j < n - i -1;j++){
            char* ilk = t[j]->takim->isim;
            char* ikinci = t[j+1]->takim->isim;
            if(kucukMu(ikinci,ilk)){//ikinci ilkten kucuk mu
                Takim geciciTakim = t[j]->takim;
                t[j]->takim = t[j+1]->takim;
                t[j+1]->takim = geciciTakim;
            }
        }
    }
    printf("Alfabetik sıralama başarılı.\n");
}

void puanSirasinaGoreSirala(Tablo* t){
    int n = ayarlar[TakimSayisi];
    for(int i = 0; i < n-1; i++){//Bubble short
        for(int j = 0; j < n - i -1;j++){
            if(t[j]->takim->puan < t[j+1]->takim->puan){
                Takim geciciTakim = t[j]->takim;
                t[j]->takim = t[j+1]->takim;
                t[j+1]->takim = geciciTakim;
            }
        }
    }
    //Puan sırasına gore siralandı ancak averaj göz onune alınarak yeniden sıralanmalı
    for(int i = 0; i < n-1; i++){//Bubble short
        for(int j = 0; j < n - i -1;j++){
            int averajJ0 = t[j]->takim->atilanGol-t[j]->takim->yedigiGol;
            int averajJ1 = t[j+1]->takim->atilanGol-t[j+1]->takim->yedigiGol;
            if(t[j]->takim->puan == t[j+1]->takim->puan && averajJ0 < averajJ1){
                Takim geciciTakim = t[j]->takim;
                t[j]->takim = t[j+1]->takim;
                t[j+1]->takim = geciciTakim;
            }
        }
    }
    printf("Puana göre sıralama başarılı.");
}
void tabloyuEkranaYaz(Tablo* t){
    printf("\n%-2s %-2s %-15.15s\t%3s %3s %3s %3s %3s %3s %3s %4s\n","S","TA","Takim Ismi","OM", "G", "B", "M", "AG","YG","AV","Puan");
    printf("------------------------------------------------------------\n");
    for(int i = 0; i < ayarlar[TakimSayisi]; i++){
        char takmaAd = t[i]->takim->takmaAd;
        char* isim = t[i]->takim->isim;
        int om = t[i]->takim->oynadigiMacSayisi;
        int g = t[i]->takim->galibiyet;
        int b = t[i]->takim->beraberlik;
        int m = t[i]->takim->maglubiyet;
        int ag = t[i]->takim->atilanGol;
        int yg = t[i]->takim->yedigiGol;
        int averaj = t[i]->takim->atilanGol-t[i]->takim->yedigiGol;
        int puan = t[i]->takim->puan;
        printf("%-2d %-2c %-15.15s\t%3d %3d %3d %3d %3d %3d %3d %3d\n", t[i]->sira, takmaAd, isim, om, g, b, m , ag, yg, averaj, puan);
        printf("------------------------------------------------------------\n");
    }
}

Tablo tabloOlustur(int sira,Takim takim){
    Tablo this = (Tablo) malloc(sizeof(struct TABLO));
    this->sira = sira;
    this->takim = takim;
    return this;
}

Tablo* takimListesiTablosunuOlustur(){//Takma ad sirasina gore tabloyu olustur.
    Tablo *t = (struct TABLO **) malloc(sizeof(struct TABLO*)*ayarlar[TakimSayisi]);//gerekli yeri ayır
    for(int i = 0; i < ayarlar[TakimSayisi]; i++){
        t[i] =tabloOlustur(i+1,takimlar[i]);
    }
    return t;
}

Tablo* puanDurumuTablosu(int secenek){
    Tablo *t = takimListesiTablosunuOlustur();

    switch(secenek) {
        case PuanSirasina:puanSirasinaGoreSirala(t); return t; break;

        case TakimlarinAlfabetikSirasina:takimlarinAlfabetikSirasinaGoreSirala(t); return t; break;

        case TakmaIsimSirasina: return t; break;

        default: printf("Yanlis secenek.");
    }
    return t;
}

void dosyadanMacOku(){
    char* dosyaAdi = "maclar1.txt";
    char* maclarDosyasi = dosyayiOku(dosyaAdi);
    if(maclarDosyasi){
        char* satirOp = "\n";
        char* bosluk = " ";
        char* macBilgileri = strtok(maclarDosyasi,satirOp);
        while(macBilgileri != NULL){
            char evSahibiTakim,misafirTakim;
            int evSahibiGol,misafirGol;
            int sayi = sscanf(macBilgileri," %c %d %c %d",&evSahibiTakim,&evSahibiGol,&misafirTakim,&misafirGol);// gelen satırı parçala
            if(sayi != 4){
                fprintf(stderr, "Mac listesinde hatalı satır\n");
            }
            macIsle(evSahibiTakim,evSahibiGol,misafirTakim,misafirGol);// alınan bilgileri işle
            macBilgileri = strtok(NULL,satirOp);//Sonraki satırı al
        }
        free(maclarDosyasi);//isi bitince bellegi serbest bırak
    }else{
        printf("\nMacların bulunduğu dosya okunamadı.");
    }
}

void klavyedenMacGir(){
    char evSahibiTakim,misafirTakim;
    int evSahibiGol,misafirGol;
    printf("Maçı formata uygun giriniz:");
    scanf(" %c %d %c %d",&evSahibiTakim,&evSahibiGol,&misafirTakim,&misafirGol);
    macIsle(evSahibiTakim, evSahibiGol, misafirTakim, misafirGol);
}

void macIsle(char evSahibiTakim,int evSahibiGol,char misafirTakim,int misafirGol){
    int evSahibiSira = evSahibiTakim % 65;//Asci karakrter karsılıgı 65ten basladığı için %65 sirayı verir
    int misafirSira = misafirTakim % 65;
    //Daha onceden mac yapidi mi kontrol et
    char *macYapildiMi;
    macYapildiMi = strchr(takimlar[evSahibiSira]->kendiSahasindaMacYaptigiTakimlar,misafirTakim);
    if(macYapildiMi){//mac yapidiysa uyarı ver
        printf("Uyarı! : Bu maç daha önce gerçekleşti : %c %d %c %d\n",evSahibiTakim,evSahibiGol,misafirTakim,misafirGol);
    }else{//Daha once mac yapilmadiysa
        //printf("%c %d %c %d\n",evSahibiTakim,evSahibiGol,misafirTakim,misafirGol); islenen macları ekrana basmak istenirse kullanılacak
        if(evSahibiGol > misafirGol){// ev sahibi kazandi
            takimlar[evSahibiSira]->puan += ayarlar[GalibiyetPuan];
            takimlar[evSahibiSira]->oynadigiMacSayisi++;
            takimlar[evSahibiSira]->galibiyet++;
            takimlar[evSahibiSira]->atilanGol += evSahibiGol;
            takimlar[evSahibiSira]->yedigiGol += misafirGol;
            takimlar[evSahibiSira]->kendiSahasindaMacYaptigiTakimlar[takimlar[evSahibiSira]->icSahadaMacSayisi] = misafirTakim;
            takimlar[evSahibiSira]->icSahadaMacSayisi++;

            takimlar[misafirSira]->puan += ayarlar[MaglubiyetPuan];
            takimlar[misafirSira]->oynadigiMacSayisi++;
            takimlar[misafirSira]->maglubiyet++;
            takimlar[misafirSira]->atilanGol += misafirGol;
            takimlar[misafirSira]->yedigiGol += evSahibiGol;

        }else if (evSahibiGol < misafirGol){//misafir kazandı
            takimlar[evSahibiSira]->puan += ayarlar[MaglubiyetPuan];
            takimlar[evSahibiSira]->oynadigiMacSayisi++;
            takimlar[evSahibiSira]->maglubiyet++;
            takimlar[evSahibiSira]->atilanGol += evSahibiGol;
            takimlar[evSahibiSira]->yedigiGol += misafirGol;
            takimlar[evSahibiSira]->kendiSahasindaMacYaptigiTakimlar[takimlar[evSahibiSira]->icSahadaMacSayisi] = misafirTakim;
            takimlar[evSahibiSira]->icSahadaMacSayisi++;

            takimlar[misafirSira]->puan += ayarlar[GalibiyetPuan];
            takimlar[misafirSira]->oynadigiMacSayisi++;
            takimlar[misafirSira]->galibiyet++;
            takimlar[misafirSira]->atilanGol += misafirGol;
            takimlar[misafirSira]->yedigiGol += evSahibiGol;


        }else {//Berabere
            takimlar[evSahibiSira]->puan += ayarlar[BeraberlikPuan];
            takimlar[evSahibiSira]->oynadigiMacSayisi++;
            takimlar[evSahibiSira]->beraberlik++;
            takimlar[evSahibiSira]->atilanGol += evSahibiGol;
            takimlar[evSahibiSira]->yedigiGol += misafirGol;
            takimlar[evSahibiSira]->kendiSahasindaMacYaptigiTakimlar[takimlar[evSahibiSira]->icSahadaMacSayisi] = misafirTakim;
            takimlar[evSahibiSira]->icSahadaMacSayisi++;

            takimlar[misafirSira]->puan += ayarlar[BeraberlikPuan];
            takimlar[misafirSira]->oynadigiMacSayisi++;
            takimlar[misafirSira]->beraberlik++;
            takimlar[misafirSira]->atilanGol += misafirGol;
            takimlar[misafirSira]->yedigiGol += evSahibiGol;
        }

    }

}

void takimlariYokEt(){
    if(takimlar==NULL)return;
    free(takimlar);
}

Takim takimOlustur(char takmaAd,char* isim){
    Takim this =(Takim) malloc(sizeof(struct TAKIM));
    this->takmaAd = takmaAd;
    this->isim = isim;
    this->puan = 0;
    this->oynadigiMacSayisi = 0;
    this->icSahadaMacSayisi = 0;
    this->galibiyet = 0;
    this->beraberlik = 0;
    this->maglubiyet = 0;
    this->atilanGol = 0;
    this->yedigiGol = 0;
    this->kendiSahasindaMacYaptigiTakimlar = (char*)calloc(ayarlar[TakimSayisi],sizeof(char));
    return this;
}

void takimlariOlustur(){
    char* dosyaAdi = "takimlar.txt";
    char* takimlarDosyasi = dosyayiOku(dosyaAdi);
    takimlar =(struct TAKIM **) malloc(sizeof(struct TAKIM*)*ayarlar[TakimSayisi]);

    if(takimlarDosyasi){
        char* satirOp = "\n";
        char* takimAdi = strtok(takimlarDosyasi,satirOp);
        char harf = 'A';//Baslangic harfi A
        for(int i = 0;i < ayarlar[TakimSayisi];i++){
            takimlar[i] = takimOlustur((harf+i),takimAdi);
            takimAdi = strtok(NULL,satirOp);
        }
        printf("Takımlar dosyadan okundu ve oluşturuldu.");
    }else{
        printf("Takımları okurken hata!");
    }
}

void ayarlariAl(){
    char* dosyaAdi = "ayarlar.txt";
    char* ayarlarDosyasi = dosyayiOku(dosyaAdi);

    if(ayarlarDosyasi){
        char* satirOp = "\n";
        char* sayi = strtok(ayarlarDosyasi,satirOp);
        for(int i=0;i < 4;i++){
            ayarlar[i] = atoi(sayi);
            sayi = strtok(NULL,satirOp);
        }
        printf("Ayarlar dosyadan alındı.\n");
    }else{
        printf("Ayarlar okurken hata!");
    }
}

char* dosyayiOku(char* dosyaAdi){
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(dosyaAdi, "r");

   if (handler)
   {
        // Dosyanın son baytını ara
        fseek(handler, 0, SEEK_END);
        // İlk bayttan son bayta veya başka bir deyişle ofset dosya boyutu
        string_size = ftell(handler);
        // Dosyanın başına dön
        rewind(handler);
        //dosyayı tutabilecek boyutta yer ayır
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
        //Tek seferde dosyayı oku
        read_size = fread(buffer, sizeof(char), string_size, handler);
        //bufferin sonunu koy
        buffer[string_size] = '\0';
        if (string_size != read_size)
        {
           // Sorun oluştuysa null atamalıyız. program hata vermemesi için
           free(buffer);
           buffer = NULL;
        }
        //dosyayı kapat
        fclose(handler);
    }else{
        printf("%s okunamadı",dosyaAdi);
        return NULL;
    }

    return buffer;
}

void turkceKarakterDuzelt(){
    setlocale(LC_ALL, "Turkish");
}
