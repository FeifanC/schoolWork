// atexit(report_mem_leak)

/* COP 3502C Assignment 1
This program is written by: Javier Perez */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "leak_detector_c.h"

typedef struct monster
{
  char *name;
  char *element;
  int population;
} monster;

typedef struct region
{
  char *name;
  int nmonsters;
  int total_population;
  monster **monsters;
} region;

typedef struct itinerary
{
  int nregions;
  region **regions;
  int captures;
} itinerary;

typedef struct trainer
{
  char *name;
  itinerary *visits;
} trainer;

monster *makeMonster(char *name, char *element, int population)
{

  monster *amonster = (monster *)malloc(sizeof(monster));
  amonster->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
  amonster->element = (char *)malloc(sizeof(element) * (strlen(element) + 1));

  strcpy(amonster->name, name);
  strcpy(amonster->element, element);

  amonster->population = population;

  return amonster;
}

monster **readMonsters(FILE *infile, int *monsterCount)
{

  char n[15], e[15];
  int p;
  monster **a_array = malloc(*monsterCount * sizeof(monster *));

  for (int i = 0; i < *monsterCount; i++)
  {
    fscanf(infile, "%s %s %d", n, e, &p);
    printf("%s %s %d\n", n, e, p);
    a_array[i] = makeMonster(n, e, p);
  }

  return a_array;
}

region **readRegions(int *countRegions, monster **monsterList, int monsterCount)
{
}

trainer *loadTrainers(int *trainerCount, region **regionList, int countRegions)
{
}

void processInputs(monster **monsterList, int monsterCount, region **regionList,
                   int regionCount, trainer *trainerList, int trainerCount)
{
}

void releaseMemory(monster **monsterList, int monsterCount, region **regionList,
                   int regionCount, trainer *trainerList, int trainerCount)
{
}

int main()
{
  int monsterCount;
  char name[15];

  FILE *fp = fopen("in.txt", "r");
  fscanf(fp, "%d %s", &monsterCount, name);
  printf("%d monsters\n", monsterCount);
  monster **mptr = readMonsters(fp, &monsterCount);

  printf("%s %s %d\n", (mptr)[3]->name, (mptr)[3]->element, (mptr)[3]->population);

  fclose(fp);
  return 0;
}
