#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

#define banner "                                ______________________________________ \n"\
"                               |                                      |\n"\
"                    _.---------|.--.   LOS MEJORES SECRETOS,          |\n"\
"                 .-'  `       .'/  ``   NUNCA LOS VERAS,              |\n"\
"              .-'           .' |    /|                                |\n"\
"           .-'         |   /   `.__//  POR QUE SIEMPRE SE ENCUENTRAN  |\n"\
"        .-'           _.--/        /  FRENTE A TI, ESTIMADO USUARIO   |\n"\
"       |        _  .-'   /        /  COMUN.                           |\n"\
"       |     ._  \\      /     `  /                                    |\n"\
"       |        ` .    /     `  /                                     |\n"\
"       |         \\ \\ '/        /                                      |\n"\
"       |        - \\  /        /|                                      |\n"\
"       |        '  .'        / |                                      |\n"\
"       |          '         |.'|                     By: Desmon       |\n"\
"       |                    |  |                                      |\n"\
"       |                    |  |______________________________________|\n"\
"       |                    |.'\n"                                      \
"       |                    /\n"                                         \
"       |                   /\n"                                           \
"       |                  /\n"                                            \
"       )                 /|\n"                                            \
"    .A/`-.              / |\n"                                            \
"   AMMMA. `-._         / /\n"                                             \
"  AMMMMMMMMA. `-.     / /\n"                                              \
" AMMMMMMMMMMMMA. `.    /\n"                                              \
"AMMMMMMMMMMMMMMMMA.`. /\n"                                               \
"MMMMMMMMMMMMMMMMMMMA.`.\n"                                                \
"MMMMMMMMMMMMMMMMMMMMMA.`.\n"                                              \
"MMMMMMMMMMMMMMMMMMMMMMMA.\n"                                              \
"MMMMMMMMMMMMMMMMMMMMMMMMMA.\n"                                            \
"MMMMMMMMMMMMMMMMMMMMMMMMMMM\n"                                           \
"MMMMMMMMMMMMMMMMMMMMMMMMMV\n"                                            


//#define __NumeroMinimoDeTamanoBloques_ 1
//#define __NumeroMaximoDeTamanoBloques_ 2
#define __MAX_VALUE_CHAR 128 // el mayor caracter a considerar
#define __LOW_VALUE_CHAR 33  // el menor caracter a considerar
// numero de caracteres a poder usarse =  255 - (32+128) = 95

uint8_t __NumeroMinimoDeTamanoBloques_ = 1;
uint8_t __NumeroMaximoDeTamanoBloques_ = 2;

// direcion de memoria donde empezar a generar direciones
// de memoria aleatorias
uint64_t __DirecionDeMemoriaMin;
// direcion limite de memoria a generar de forma aletoria
uint64_t __DirecionDeMemoriaMax;
// las direciones de memoria aleaotiras se generan en el rango
// __DirecionDeMemoriaMin y __DirecionDeMemoriaMax


// estructura donde almacenar la direcion de memoria donde
// se empezara(posicionInicial) a guardar el string text y 
// donde acabara(posicionFinal)
struct lugar
{
    uint64_t posicionInicial;
    uint64_t posicionFinal;
    uint8_t *text;
} typedef lugar;

// estructura donde almacenar la exxtension del archivo
// los bytes de la cabezera y el nombre ascii
struct extension
{
    uint8_t *extension;
    uint8_t cabezera[6];
    uint8_t *name;
} typedef extension;

// lista de etension con cabezeras y ascii
#define ZIP   {(uint8_t*)".zip",  {0x50, 0x4B, 0x03, 0x04},             (uint8_t*)"PK"}
#define RAR   {(uint8_t*)".rar",  {0x52, 0x61, 0x72, 0x21},             (uint8_t*)"Rar!"}
#define TAR   {(uint8_t*)".tar",  {0x1F, 0x8B, 0x08, 0x00},             (uint8_t*)""}
#define TGZ   {(uint8_t*)".tgz",  {0x1F, 0x9D, 0x90, 0x70},             (uint8_t*)""}
#define DOC   {(uint8_t*)".doc",  {0xD0, 0xCF, 0x11, 0xE0},             (uint8_t*)"ÐÏ.à"}
#define XLS   {(uint8_t*)".xls",  {0xD0, 0xCF, 0x11, 0xE0},             (uint8_t*)""}
#define PDF   {(uint8_t*)".pdf",  {0x25, 0x50, 0x44, 0x46},             (uint8_t*)"%PDF"}
#define WMV   {(uint8_t*)".wmv",  {0x30, 0x26, 0xB2, 0x75},             (uint8_t*)""}
#define FLV   {(uint8_t*)".flv",  {0x46, 0x4C, 0x56, 0x01},             (uint8_t*)"FLV"}
#define BMP1  {(uint8_t*)".bmp",  {0x50, 0x4B, 0x03, 0x04},             (uint8_t*)"BM"}
#define BMP2  {(uint8_t*)".bmp",  {0x50, 0x4B, 0x62, 0x25},             (uint8_t*)"BMp%"}
#define BMP3  {(uint8_t*)".bmp",  {0x50, 0x4B, 0x76, 0x03},             (uint8_t*)"BMv"}
#define GIF1  {(uint8_t*)".gif",  {0x47, 0x49, 0x46, 0x38, 0x39, 0x61}, (uint8_t*)"GIF89a"}
#define GIF2  {(uint8_t*)".gif",  {0x47, 0x49, 0x46, 0x38, 0x37, 0x61}, (uint8_t*)"GIF87a"}
#define ICO   {(uint8_t*)".ico",  {0x00, 0x00, 0x01, 0x00},             (uint8_t*)""}
#define JPEG1 {(uint8_t*)".jpeg", {0xFF, 0xD8, 0xFF, 0xE0},             (uint8_t*)"JFIF"}
#define JPEG2 {(uint8_t*)".jpeg", {0xFF, 0xD8, 0xFF, 0xFE},             (uint8_t*)"JFIF"}
#define PNG   {(uint8_t*)".png",  {0x89, 0x50, 0x4E, 0x47},             (uint8_t*)"PNG"}
#define SFW1  {(uint8_t*)".sfw",  {0x43, 0x57, 0x53, 0x06},             (uint8_t*)"Cws"}
#define SFW2  {(uint8_t*)".sfw",  {0x43, 0x57, 0x53, 0x08},             (uint8_t*)"Cws"}
#define MP31  {(uint8_t*)".mp3",  {0x49, 0x44, 0x33, 0x2E},             (uint8_t*)"ID3"}
#define MP32  {(uint8_t*)".mp3",  {0x49, 0x44, 0x33, 0x03},             (uint8_t*)"ID3"}
#define EXE1  {(uint8_t*)".exe",  {0x25, 0x50, 0x44, 0x46},             (uint8_t*)"MZP"}
#define EXE2  {(uint8_t*)".exe",  {0x4D, 0x5A, 0x50, 0x00},             (uint8_t*)"MZ"}
#define DLL   {(uint8_t*)".dll",  {0x4D, 0x5A, 0x90, 0x00},             (uint8_t*)"MZ"}
#define BIN   {(uint8_t*)".bin",  {0x7F, 0x45, 0x4C, 0x46},             (uint8_t*)"ELF"}


/*
.ZIP	50 4B 03 04	PK
.RAR	52 61 72 21	Rar!
.TAR	1F 8B 08 00	
.TGZ	1F 9D 90 70	
.DOC	D0 CF 11 E0	ÐÏ.à
.XLS	D0 CF 11 E0	
.PDF	25 50 44 46	%PDF
.WMV	30 26 B2 75	
.FLV	46 4C 56 01	FLV
.BMP	42 4D F8 A9/ 62 25 / 76 03	BM, BMp% , BMv
.GIF	47 49 46 38 39 61 / 37 61	GIF89a GIF87a
.ICO	00 00 01 00	
.JPEG	FF D8 FF E0 / FE	JFIF
.PNG	89 50 4E 47	PNG
.SFW	43 57 53 06 / 08	Cws
.MP3	49 44 33 2E /03	ID3
.EXE	4D 5A 50 00 /90 00	MZP / MZ
.DLL	4D 5A 90 00	MZ
Linux bin(bin)	7F 45 4C 46	ELF
*/

// puntero para generar el array de tamano dinamico donde se almacenara
// elementos del tipo lugar
lugar *lugares;


// declaracion de funciones:
uint64_t generarDirecionDeMemoria(uint64_t min, uint64_t max);
bool writeFile(uint8_t *file_contents, uint64_t size, FILE *in_file);
void DesfragmentacionDeDatos(uint8_t *buffer, uint64_t sizeString);


// codigo main
int main()
{
    extension rar   = RAR;
    extension tar   = TAR;
    extension zip   = ZIP;
    extension tgz   = TGZ;
    extension doc   = DOC;
    extension xls   = XLS;
    extension pdf   = PDF;
    extension wmv   = WMV;
    extension flv   = FLV;
    extension bmp1  = BMP1;
    extension bmp2  = BMP2;
    extension bmp3  = BMP3;
    extension gif1  = GIF1;
    extension gif2  = GIF2;
    extension ico   = ICO;
    extension jpeg1 = JPEG1;
    extension jpeg2 = JPEG2;
    extension png   = PNG;
    extension sfw1  = SFW1;
    extension sfw2  = SFW2;
    extension mp31  = MP31;
    extension mp32  = MP32;
    extension exe1  = EXE1;
    extension exe2  = EXE2;
    extension dll   = DLL;
    extension bin   = BIN;


    extension extensiones[] = {
        zip, rar, tar, tgz, doc, xls,
        pdf, wmv, flv, bmp1, bmp2, bmp3,
        gif1, gif2, ico, jpeg1, jpeg2, png,
        sfw1, sfw2, mp31, mp32, exe1, exe2,
        dll, bin
    };


    // variable donde almacenar el archivo a usar para
    // realizar esteganografia
    uint8_t filename[20];
    //filename = "estego.jpg";

    // recepcion de los datos

    puts(banner);

    printf("[!] Nombre del archivo a usar: ");
    scanf("%[^\n]s", filename);
    //puts("");
    flush();
    
    // variable donde almacenar los datos a esconder
    uint8_t data[] = "Hola Mundo, aqui tito desmon, si encontraste esto enorabuena, has roto mi algoritmo de esteganografia : 'v";
    printf("[!] Datos a introducir: ");
    //scanf("%[^\n]s", data);
    //puts("");
    flush();


    printf("[!] Numero Minimo De Tamano de Bloques (recomendado 1): ");
    scanf("%hdd", &__NumeroMinimoDeTamanoBloques_);
    //puts("");
    flush();

    printf("[!] Numero Maximo De Tamano de Bloques (recomendado 2): ");
    scanf("%hdd", &__NumeroMaximoDeTamanoBloques_);
    puts("");

    // generar una semilla aleatoria segun el tiempo que retorne
    // el sistema operativo
    srand(time(NULL));

    // apertura del archivo deseado para su lectura
    FILE *in_file = fopen((const char*)filename, "rb");

    // comprovacion de que no halla ocurrido error alguno al abrir el archivo
    if (!in_file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    struct stat sb;
    if (stat((const char*)filename, &sb) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }


    // reserva memoria dinamica para los datos del archivo
    uint8_t *file_contents = (uint8_t *)malloc(sb.st_size * sizeof(uint8_t));  
    // leer el archivo y almacenar su contenido en file_contents
    fread(file_contents, sb.st_size, 1, in_file); 

    // direcion maxima a generar = el tamano maximo del archivo
    __DirecionDeMemoriaMax = sb.st_size;

    //__DirecionDeMemoriaMin = 0x8000; // 32 Kb
    __DirecionDeMemoriaMin = 0xFFFF; // 64 Kb

    // crear archivo nuevo
    char new_filename[strlen((const char*)filename) + 10];
    strcpy(new_filename, "esteg-");
    strcat(new_filename, (const char*)filename);
    printf("Nombre del archivo de salida: %s\n", new_filename);
    printf("Tamano del archivo: %ld\n\n", sb.st_size);

    // abrir archivo nuevo en modo lectura binaria
    in_file = fopen((const char*)new_filename, "wb");

    // crear array para los structs de lugar donde se almacena cada bloque con rango de direcion de memoria
    lugares = (lugar *)malloc(sizeof(lugar) * strlen((const char*)data));
    
    // desfragmentar los bloques de datos en seciones aleatorias de memoria 
    // el tamano de los bloques de datos se genera de forma aleatoria mediante los
    // valores __NumeroMinimoDeTamanoBloques_ y __NumeroMaximoDeTamanoBloques_
    DesfragmentacionDeDatos(data, strlen((const char*)data));
    printf("-> %s\n", data);

    for (int i = 0; i < strlen((const char*)data); i++)
    {
        uint16_t z = 0;
        if (lugares[i].posicionInicial != 0)
        {
            printf(" - posicionInicial: \t0x%lx  \tposicionFinal: 0x%lx \tText: %s\n", lugares[i].posicionInicial, lugares[i].posicionFinal, lugares[i].text);
            for(int j = lugares[i].posicionInicial; j < lugares[i].posicionFinal; j++ ){
                // escribir en el contenido de la imagen caracter a caracter:
                file_contents[j] = lugares[i].text[z];
                //printf("%c", lugares[i].text[z]);
                z++;
            }
        }
    }
    puts("");

    // escribir en el archivo
    writeFile(file_contents, sb.st_size, in_file);
    fclose(in_file);

    // generar mapa:
    in_file = fopen("mapa.map", "w");
    
    for (int i = 0; i < strlen((const char *)data); i++)
    {
        uint8_t buffer[20];
        if (lugares[i].posicionInicial != 0)
        {
            sprintf(buffer, " |%ld-%ld| ", lugares[i].posicionInicial, lugares[i].posicionFinal);
            writeFile(buffer, 20, in_file);
        }
    }

    fclose(in_file);


    // liberar memoria
    free(file_contents);
    free(lugares);

    // salida del programa exitosa
    exit(EXIT_SUCCESS);
    return 0;
}

uint64_t generarDirecionDeMemoria(uint64_t min, uint64_t max)
{
    //srand((unsigned)time(NULL));
    // generar valor aleatorio entre el rango min y max
    uint64_t memori = rand() % (max + 1 - min) + min;
    return memori;
}

void DesfragmentacionDeDatos(uint8_t *buffer, uint64_t sizeString)
{

    //uint8_t * TamanoDeBloques = malloc(sizeString * sizeof(uint8_t));
    uint64_t contador = 0;

    for (uint64_t i = 0; i < sizeString; contador++)
    {

        uint8_t *bloqText = (uint8_t *)malloc(__NumeroMaximoDeTamanoBloques_ * sizeof(uint8_t)); // lugar donde almacenar el texto que contendra el bloque

        // generar tamano del bloque donde almacenar el 'pedazo' de string
        uint8_t bloq = generarDirecionDeMemoria(__NumeroMinimoDeTamanoBloques_, __NumeroMaximoDeTamanoBloques_);

        // asignar el 'pedazo' de string al nuevo bucle
        for (uint8_t j = 0; j < bloq; j++)
            bloqText[j] = buffer[j + i];

        // generar direcion de memoria aleatoria
        uint64_t posicionDeMemoria = generarDirecionDeMemoria(__DirecionDeMemoriaMin, __DirecionDeMemoriaMax);

        // posicionInicial = posicionDeMemoria    posicionFinal = posicionDeMemoria + bloq
        // text = bloqText
        lugar posicion = {posicionDeMemoria, posicionDeMemoria + bloq, bloqText};
        lugares[contador] = posicion;
        printf("posicionInicial: \t%15.ld  \tposicionFinal: %4.ld \tText: %s\n", lugares[contador].posicionInicial, lugares[contador].posicionFinal, lugares[contador].text);

        // generar una posicion de memoria aleatoria y calcular el final de la posicion
        // del bloque aleatorio
        bloqText = (uint8_t *)malloc(__NumeroMaximoDeTamanoBloques_ * sizeof(uint8_t));

        i += bloq;
    }

    //lugar * _lugares = (lugar *)malloc(sizeof(lugar) * contador);
    //for (uint8_t j = 0; j < contador; j++) _lugares[j] = lugares[j];
    //free(lugares);
}

bool writeFile(uint8_t *file_contents, uint64_t size, FILE *in_file)
{
    // escribir en el archivo in_file caracter a caracter.
    for (uint64_t i = 0; i < size; i++)
        fputc(file_contents[i], in_file); // escribir caracter a caracter
    return true;
}
