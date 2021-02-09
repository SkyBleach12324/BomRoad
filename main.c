#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define nrows 10
#define ncols 10

#define MAP_SIZE 25

int main(void) 
{
  FILE *f;
  int fraw[2048];
  char input[256];
  char filename[256];  
  // aloca um vector de vectores com tamanho nrows
char **map = (char **) malloc(nrows * sizeof(char *));
int DIMX, DIMY = 25;
if (map == NULL)
{
    puts("Error: Out of Memory");
    exit(1);
}
// para cada elemento do vector, aloca um vector com tamanho ncols
for(DIMX = 0; DIMX < nrows; DIMX++)
{
    map[DIMX] = malloc(ncols * sizeof(char));
    if (map[DIMX] == NULL)
    {
        puts("Error: Out of Memory");
        exit(1);
    }
}    
// a partir daqui pode aceder aos elementos da matriz da forma habitual map[x][y]

   char menu[] = "+-----------------------------------------------------\n"
                "read <filename>    - read input file\n"
                "show               - show the mine map\n"
                "trigger <x> <y>    - trigger mine at <x> <y>\n"
                "log <x> <y>		   - trigger mine at <x> <y>\n"                
                "plant <x> <y>      - place armed mine at <x> <y>\n"
                "export <filename>  - save file with current map\n"
                "quit               - exit program\n"
                "sos                - show menu\n"
                "+-----------------------------------------------------\n";

  printf("%s", menu);

  while (1){
    //limpar input indesejado
    fflush(stdin);
    //pedir input do utilizador
    printf(">");
    scanf("%s", input);
    
    //ler ficheiro
    if (!strcmp(input, "read")){
      scanf("%s", filename);
      f = fopen(filename, "r");
      if (f == NULL){
        printf("Error opening file\n");
        continue;
      } else {
        for (int DIMX = 0; DIMX < **map; DIMX++)
        {
          for (int DIMY = 0; DIMY <= **map; DIMY++)
          {
            map[DIMX][DIMY] = '_';
          }
        }
        int x, y, k;
        int i = 0;
        while (fscanf(f, "%d", &k)==1)
        {
          fraw[i] = k;
          i++;          
        }


        if (i %2 == 0){
          for (int j = 0; j < i; j++){
            x = fraw[j];
            y = fraw[j+1];

            //testar se as coordernadas são validas

            if (x < **map && y < **map && x >= 0 && y >= 0)
            {
              map[x][y] = '.';
            } 
            else { 
              printf("File is corrupted\n");
              break;
            }
            //incrementar j 2 vezes, uma para x e outra para y
            j++;          
          }
        } 
        else printf("File is corrupted\n");

        fclose(f);
      }
      //imprimir o mapa um caracter de cada vez
    } 
    else if (!strcmp(input, "show"))
    {
      for (int i = 0; i < DIMX; i++)
      {
        for (int j = 0; j < DIMY; j++)
        {
          printf("%c", map[i][j]);
        }
        printf("\n");
      }
      //ativar mina
    } 
    else if (!strcmp(input, "trigger"))
    {
      int x, y;
      scanf("%d%d", &x, &y);

      //verificar se as coordenadas sao validas

      if (x < MAP_SIZE && y < MAP_SIZE && x >= 0 && y >= 0)
      {
        //caso as coordenadas serem validas, verificar se existe uma mina para ativar
        if (map[x][y] == '.' || map[x][y] == '*')
        {
          map[x][y] = '*';
        } 
        else printf("No mine at specified coordinate\n");
      } 
      else printf("Invalid coordinate\n");
      //criar mina
      
    } 
    else if (!strcmp(input, "plant")){
      int x, y;
      scanf("%d%d", &x, &y);
      //verificar se as coordenadas sao validas
      if (x < MAP_SIZE && y < MAP_SIZE && x >= 0 && y >= 0)
      {
        map[x][y] = '.';
      } 
      else printf("Invalid coordinate\n");
      //exportar mapa para ficheiro

    } 
    else if (!strcmp(input, "export"))
    {
      //receber nome do ficheiro
      scanf("%s", filename);
      //abrir ou criar ficheiro com o nome recebido
      f = fopen(filename, "w");
      //testar se o ficheiro foi corretamente criado
      if (f == NULL)
      {
        continue;
      }
      for (int i = 0; i < MAP_SIZE; i++)
      {
        for (int j = 0; j < MAP_SIZE; j++)
        {
          if (map[i][j] == '.' || map[i][j] == '*')
          {
            //imprimir uma mina em cada linha
            fprintf(f, "%d %d\n", i, j);
          }
        }
      }
      //fechar o ficheiro
      fclose(f);
      //imprimir menu
    } 
    else if (!strcmp(input, "sos"))
    {
      printf("%s", menu);
      //sair do programa
    } 
    else if (!strcmp(input, "quit"))
    {
      return 0;
      //indicar que é um input invalido
    } 
    else printf("Invalid input\n");
  }
}
