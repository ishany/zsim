#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>
#include <limits.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <math.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::min
#include "visualize.h"
using namespace std;
extern unsigned char* data_array;

typedef struct Line{
	 char Byte[64];
}Line;

// calculate size
int countWidth(unsigned short int num);
int countLine(Line line);
int countFlip(Line line);
// get data from array
Line cTol(int index);
Line cTol_sub(int index);
Line cTol_orig(int index);
Line cTol_perf(int index);
// insert into array
Line compare(Line line, uint32_t tag);
Line compare_perfect(Line line, uint32_t tag);
void LongToLine (Line line, uint32_t tag);
// operation for data structure
Line add(Line l1, Line l2);
Line xxor(Line l1, Line l2);
Line sub(Line l1, Line l2);
// helper function
int getTag(long long line);
void printline(Line line);
int Empty(Line line);
double best_compression_rate();
double best_possible_rate();
Line compare_perfect(Line line, uint32_t tag);

void axorb(char* c,char* a, char* b);
void linetochar(char* block, Line line);
long long unsigned * convertBuffer2Array (long long unsigned* values, char * buffer, unsigned size, unsigned step);
unsigned BDICompress (char * buffer, unsigned _blockSize);
double calcRate(int alloc, int comp, int orig_flip, int flip);
double calcRatePerf(int alloc, int comp, int orig_flip, int flip);
double calcRate_raw(int alloc, int comp, int orig_flip, int flip);
double calcRatePerf_raw(int alloc, int comp, int orig_flip, int flip);
Line compress(Line line);

// 
Line compare_sub(Line line, uint32_t tag);
