#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node 
{
  int time;
  int x;
  int y;
  struct Node* next;
};

void explode(int x, int y, char **map, int nrows, int ncols);
void addToList(struct Node** head, int timenew, int xnew, int ynew);
char **reallocMap (char **arr, int nRows, int nCols);

int main(void) {
  //var def
  FILE *f;
  int fraw[2048];
  int nrows = 25;
  int ncols = 25;
  char input[256];
  char fname[256];  
  char menu[] = "+-----------------------------------------------------\nread <filename> - read input file\nshow - show the mine map\ntrigger <x> <y> - trigger mine at <x> <y>\nlog <x> <y> - trigger mine at <x> <y>\nplant <x> <y> - place armed mine at <x> <y>\nexport <filename> - save file with current map\nquit - exit program\nsos - show menu\n+-----------------------------------------------------\n";

  struct Node* head = NULL;
  head = (struct Node*) malloc(sizeof(struct Node));
  head->time = 0;

  char **map;
  map = malloc( sizeof *map * nrows );
  if ( map )
  {
    for ( int i = 0; i < nrows; i++ )
      map[i] = malloc( sizeof *map[i] * ncols);
  }
  for (int x = 0; x < nrows; x++){
    for(int y = 0; y < ncols; y++){
      map[x][y] = '_';
    }
  }

  printf("%s", menu);

  while (1)
  {
    //clean non wanted inputs
    fflush(stdin);
    //ask users input
    printf(">");
    scanf("%s", input);    
    //read file
    if (!strcmp(input, "read"))
    {
      //receive name file
      scanf("%s", fname);
      //open file
      f = fopen(fname, "r");
      //if doesnt exist, error opening file
      if (f == NULL)
      {
        printf("Error opening file\n");
        continue;
      } else {
        int x, y, k;
        int i = 0;
        //read file numbers
        while (fscanf(f, "%d", &k)==1)
        {
          if (i == 0)
          {
            nrows = k;
          } 
          else if (i == 1)
          {
            ncols = k;
          } 
          else 
          {
            fraw[i] = k;
          }
          i++;        
        }
        //delete old map
        map = reallocMap(map, nrows, ncols);
        for (int i = 0; i < nrows; i++)
        {
          for (int j = 0; j <= ncols; j++)
          {
            map[i][j] = '_';
          }
        }
        //read pars
        if (i %2 == 0)
        {
          for (int j = 2; j < i; j++)
          {
            x = fraw[j];
            y = fraw[j+1];
            //test if coordinates are valid
            if (x < nrows && y < ncols && x >= 0 && y >= 0)
            {
              //put mines in coordinates
              map[x][y] = '.';
            } 
            else 
            {
              printf("File is corrupted\n");
              break;
            }
            //increment j 2 times for x and y
            j++;          
          }
        }
         else printf("File is corrupted\n");
        //close file
        fclose(f);
      }
      //print map, a caracter a time
    } 
    else if (!strcmp(input, "show"))
    {
      for (int i = 0; i < nrows; i++)
      {
        for (int j = 0; j < ncols; j++)
        {
          printf("%c", map[i][j]);
        }
        printf("\n");
      }
      //activate mine
    } 
    else if (!strcmp(input, "trigger"))
    {
      int x, y;
      scanf("%d%d", &x, &y);
      //see if coordinates are valid
      if (x < nrows && y < ncols && x >= 0 && y >= 0) 
      {
        if (map[x][y] == '*' || map[x][y] == '.')
        {
          explode(x, y, map, nrows, ncols);
        } 
        else printf("No mine at specified coordinate\n");
      } 
      else printf("Invalid coordinate\n");
      //create mine
    } 
    else if (!strcmp(input, "log"))
    {
      int x, y;
      scanf("%d%d", &x, &y);
      //see if coordinates are valid
      if (x < nrows && y < ncols && x >= 0 && y >= 0)
      {
        struct Node* node = head;
        //logExplode(&head, time, x, y, map);
        while (node != NULL) 
        {
          map[node->x][node->y] = '*';
          printf("%d [%d,%d]\n", node->time, node->x, node->y);
          node = node->next;
        }
      }
    }
    else if (!strcmp(input, "plant"))
    {
      int x, y;
      scanf("%d%d", &x, &y);
      //see if coordinates are valid
      if (x < nrows && y < ncols && x >= 0 && y >= 0)
      {
        map[x][y] = '.';
      } else printf("Invalid coordinate\n");
      //export map to file
    } 
    else if (!strcmp(input, "export"))
    {
      //receive name file
      scanf("%s", fname);
      //open or create file with the wanted name
      f = fopen(fname, "w");
      //test if the file was created correctly
      if (f == NULL)
      {
        continue;
      }
      for (int i = 0; i < nrows; i++)
      {
        for (int j = 0; j < ncols; j++)
        {
          if (map[i][j] == '.' || map[i][j] == '*')
          {
            //print mine each line
            fprintf(f, "%d %d\n", i, j);
          }
        }
      }
      //close file
      fclose(f);
      //print menu
    }
    else if (!strcmp(input, "quit"))
    {
      return 0;
      //indicate that it is a invalid input
    } 
     else if (!strcmp(input, "sos"))
    {
      printf("%s", menu);
      //exit
    } 
    else printf("Invalid input\n");
  }
}

//explode mines
void explode(int x, int y, char **map, int nrows, int ncols) 
{
    //check adjacent positions for mine occurrences
    for (int xi = -1; xi <= 1; xi++)
    {
      for (int yi =-1; yi <= 1; yi++) 
      {
        if ((x+xi >= 0 && y+yi >= 0) && (x+xi < nrows && y+yi < ncols)) 
        {
          if (map[x+xi][y+yi] == '.') 
          {
            //explode mines
            map[x+xi][y+yi] = '*';
            explode(x+xi,y+yi, map, nrows, ncols);
          }
        }
      }
    //indicate that there is no mine at the given coordinate
  } 
}

void addToList(struct Node** head, int timenew, int xnew, int ynew) 
{
  struct Node* nnode =(struct Node*) malloc(sizeof(struct Node));  
  nnode->time = timenew;
  nnode->x = xnew;
  nnode->y = ynew;

  nnode->next = (*head);
  (*head) = nnode;
}

char **reallocMap (char **arr, int nRows, int nCols) 
{
  // use a single realloc for the char pointers to the first char of each row
  // so we reallocate space for the pointers and then space for the actual rows.
  char **newArr = realloc (arr, sizeof(char *) * nRows + sizeof(char) * nCols * nRows);

  if (newArr)
   {
    // calculate offset to the beginning of the actual data space
    char *row = (char *)(newArr + nRows);
    // fix up the pointers to the individual rows
    for (int i = 0; i < nRows; i++) 
    {
      newArr[i] = row;
      row += nCols;
    }
  }
  return newArr;
}