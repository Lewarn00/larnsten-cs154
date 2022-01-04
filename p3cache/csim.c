#include "cachelab.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//Global variables to record all hits, misses, evictions
int hits, misses, evictions;

typedef enum {false, true} bool;

//Struct for each block within a set, holds information for valid bit and tag
typedef struct{
  int valid;
  long int tag;
  int time;
} block;

//Struct building on block, defines a set
typedef struct{
  block *BlockNum;
} set;

//Struct building on set, defines a cache
typedef struct{
  set *sets;
} Cache;

//Defines given information which will be used to create a cache
typedef struct{
  int sets;
  int lines;
  int setBits;
  int offsetBits;  
} CacheData;

//For verbose posibility
int verbose; 

//Function to create a new cache for given CacheData
Cache* newCache(CacheData Cinfo){
  hits = 0;
  misses = 0;
  evictions = 0;
  Cache *c;
  c = (Cache*)malloc(sizeof(Cache));
  c -> sets = (set*)malloc(sizeof(set)*Cinfo.sets);
  int i, j;
  for(i = 0; i< Cinfo.sets; i++){
    c -> sets[i].BlockNum = (block*)malloc(sizeof(block)*Cinfo.lines);
    for(j = 0; j < Cinfo.lines; j++){
      c -> sets[i].BlockNum[j].tag = 0;
      c -> sets[i].BlockNum[j].valid = 0;
      c -> sets[i].BlockNum[j].time = 0;
    }
  }
  return c;
}

//Summing the hits, misses and evictions
//Printing out the result if verbose called
void sumResults(char operation, bool hit, bool evict, long int address, int size){
  if(operation == 'M'){
    if(hit){
      hits = hits + 2;
      if(verbose){
        printf("%c %lx,%d hit hit\n", operation, address, size);
      }
    }else if(evict){
      misses++;
      evictions++;
      hits++;
      if(verbose){
        printf("%c %lx,%d miss eviction hit\n", operation, address, size);
      }
    }else{
      misses++;
      hits++;
      if(verbose){
        printf("%c %lx,%d miss hit\n", operation, address, size);
      }
    }
  }else{
    if(hit){
      hits++;
      if(verbose){
        printf("%c %lx,%d hit\n", operation, address, size);
      }
    }else if(evict){
      misses++;
      evictions++;
      if(verbose){
        printf("%c %lx,%d miss eviction\n", operation, address, size);
      }
    }else{
      misses++;
      if(verbose){
        printf("%c %lx,%d miss\n", operation, address, size);
      }
    }
  }
}

//Simulate function
void simulate(CacheData Cinfo, char *filepath){
  FILE *f = fopen(filepath, "r");
  Cache *c = newCache(Cinfo);
  long int address;
  int size;
  char operation;
  bool hit, miss, eviction;

  long int time = 0; 
  long int LRU; //Time and LRU used when choosing which cache line to evict

  //Scanf assigns the input to their respective variables
  while(fscanf(f, " %c %lx,%d", &operation, &address, &size) == 3){
    hit = false;
    eviction = false;
    miss = true;
    int empty = -1;

    //Skips a loop if the operation is I
    if(operation == 'I') continue;

    //Creating the tag and index from the given information
    long int tag = address >> (Cinfo.setBits + Cinfo.offsetBits);
    int index = (address >> Cinfo.offsetBits) & (Cinfo.sets - 1);
    set s = c -> sets[index];
    LRU = s.BlockNum[0].time;
    int evict = 0;

    //A loop to determine cache hits and misses 
    int i;
    for(i = 0; i < Cinfo.lines; i++){
      if(s.BlockNum[i].valid == 1){
    		if(s.BlockNum[i].tag == tag){
    		  miss = false;
    		  hit = true;
    		  s.BlockNum[i].time = time;
    		  time++;
    		}else if(s.BlockNum[i].time < LRU){
    		    LRU = s.BlockNum[i].time;
    		    evict = i;
    		}
      }else{
		    empty = i;
      }
    }

    //Eviction handling for misses 
    if(miss){
  	  if(empty == -1){
    		eviction = true;
    		s.BlockNum[evict].tag = tag;
    		s.BlockNum[evict].time = time;
    		time++;
      }else{
    		s.BlockNum[empty].valid = 1;
    		s.BlockNum[empty].tag = tag;
    		s.BlockNum[empty].time = time;
    		time++;
      }
    }

    sumResults(operation, hit, eviction, address, size);
  }

  fclose(f);

  //Freeing the cache
  int i;
  for(i = 0; i < Cinfo.sets; i++){
    free(c -> sets[i].BlockNum);
  }
  free(c -> sets);
  free(c);
}

//Function to print usage info if requested
void usageInfo(char *filepath){
  printf( "Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", filepath);
  printf( "  -h         Help flag that prints usage info\n" );
  printf( "  -v         Verbose flag that displays trace info\n" );
  printf( "  -s <s>     Number of set index bits\n" );
  printf( "  -E <E>     Associativity (number of lines per set)\n" );
  printf( "  -b <b>     Number of block bits\n" );
  printf( "  -t <tracefile>   Name of the valgrind trace to replay\n" );
}

int main(int argc, char* argv[]){
  verbose = 0;
  char *f;
  CacheData cache = {0, 0, 0, 0};

  //While loop to parse input
  int i = 1;

  while(i < argc && argv[i + 1]){
    if(strcmp("-h", argv[i]) == 0){
      usageInfo(argv[0]);
      exit(1);
      
    }else if(strcmp("-v", argv[i]) == 0){
      verbose = 1;
      i--;

    }else if(strcmp("-s", argv[i]) == 0){
      cache.setBits = atoi(argv[i + 1]);

    }else if(strcmp("-E", argv[i]) == 0){
      cache.lines = atoi(argv[i + 1]);

    }else if(strcmp("-b", argv[i]) == 0){
      cache.offsetBits = atoi(argv[i + 1]);

    }else if(strcmp("-t", argv[i]) == 0){
      f = argv[i+1];

    }else{
      fprintf(stderr, "invalid parameter\n");
      exit(1);
    }
    i += 2;
  }

  cache.sets = pow(2,cache.setBits);
  
  simulate(cache, f); 
  printSummary(hits, misses, evictions);
}
