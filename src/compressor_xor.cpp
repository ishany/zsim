#include "compressor.h"
#define numLines 32768
#define size 64
unsigned char* data_array_orig = (unsigned char*)malloc(numLines * size); // OG data array : L2 cache
unsigned char* data_array = (unsigned char*)malloc(numLines * size);      // XOR data_array
unsigned char* data_array_sub = (unsigned char*)malloc(numLines * size);  // SUB data_array
unsigned char* data_array_perf = (unsigned char*) malloc(numLines * size);// COMPLEX_PERFECT data_array
int index_ref[numLines];                                // reference to each other for XOR pair
int index_ref_sub[numLines];                            //                             SUB 
int index_ref_perf[numLines];                                 //                             COMPLEX_PERFECT
int sizes[numLines][3];                                 // sizes of entries (as for bancwidth)
int occupied[numLines][3];                              // availabilty of entries
int initialized[numLines][3];                           // status of entries
extern int uncompress_size;
/**
 * insert a line into the data_array
 *
 * @param line cache line to be inserted
 * @param tag 
 * @return a xored line
 */
static unsigned long long my_llabs ( long long x )
{
   unsigned long long t = x >> 63;
   return (x ^ t) - t;
}

#define VG_(x) x


Line compare(Line line, uint32_t tag)
{
    // A : line, iterate through the data_array (cache) to find B
    uint32_t index = 0; // iterator index
    uint32_t ret = tag; // ret: index of B
    Line min = line;    // initialize the output (A ^ B) to A
    Line compare = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    while(index < numLines)
    {	
		if(!initialized[index][0] || occupied[index][0]){   // skip all unintialized lines
			index++;
			continue;		
		}
        compare = cTol_orig(index);
	
        if((countLine(xxor(compare, line))<countLine(min)) && (index != tag) && occupied[index][0] == 0)
        {   // occupied[index][0] == 0 => not xoring with any line
            min = xxor(compare,line);
            ret =  index;  
			break;
        }
        // increment iteration
        index ++;
    }
    // if ret is not initialized: 
	if(!initialized[ret][0]){
		uncompress_size += 256;
		ret = tag;
		initialized[tag][0] = 1;	
	}
	else{
		index_ref[ret] = tag;
		index_ref[tag] = ret;
		occupied[ret][0] = 1;
		occupied[tag][0] = 1;
	}
	LongToLine (line, tag);
    sizes[ret][1] = countLine(min);
	//printf("size of line_xor: %d\n", sizes[ret][1]);
	//printline(min);
    for(int i = 0; i < size; i++){
        data_array[ret * size + i] = (unsigned char) min.Byte[i];
        data_array[tag * size + i] = (unsigned char) min.Byte[i];
    }
    return min;
}
/**
 * insert a line into the data_array_perf. multiple lines could be xored
 * with same candidate. 
 *
 * @param line cache line to be inserted
 * @param tag 
 * @return a xored line 
 */
Line compare_perfect(Line line, uint32_t tag)
{
    uint32_t index = 0;
    uint32_t ret = tag;
    Line min = line;
    Line compare = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    while(index < numLines)
    {	
		if(!initialized[index][2] || occupied[index][2]){
			index++;
			continue;		
		}
        compare = cTol_orig(index);
	
        if((countLine(xxor(compare, line))<countLine(min)) && (index != tag) && occupied[index][2] == 0)
        {
            min = xxor(compare,line);
            ret =  index;  
        }
        index ++;
    }
	if(!initialized[ret][2]){
		uncompress_size += 256;
		ret = tag;
		initialized[tag][2] = 1;	
	}
	else{
		index_ref_perf[ret] = tag;
		index_ref_perf[tag] = ret;
		occupied[ret][2] = 1;
		occupied[tag][2] = 1;
	}
	//printline(min);
    for(int i = 0; i < size; i++){
        data_array_perf[ret * size + i] = (unsigned char) min.Byte[i];
    }
    return min;
}
/**
 * @param index tag of the cache line
 * @return Line structured data_array_orig entry
 */

void linetochar(char* c, Line line) {
 c = line.Byte;
}

Line cTol_orig(int index){
    return { (data_array_orig[index * size + 63]), (data_array_orig[index * size + 62]),
             (data_array_orig[index * size + 61]), (data_array_orig[index * size + 60]),
             (data_array_orig[index * size + 59]), (data_array_orig[index * size + 58]), 
             (data_array_orig[index * size + 57]), (data_array_orig[index * size + 56]),
             (data_array_orig[index * size + 55]), (data_array_orig[index * size + 54]),
             (data_array_orig[index * size + 53]), (data_array_orig[index * size + 52]), 
             (data_array_orig[index * size + 51]), (data_array_orig[index * size + 50]),
             (data_array_orig[index * size + 49]), (data_array_orig[index * size + 48]),
             (data_array_orig[index * size + 47]), (data_array_orig[index * size + 46]), 
             (data_array_orig[index * size + 45]), (data_array_orig[index * size + 44]),
             (data_array_orig[index * size + 43]), (data_array_orig[index * size + 42]),
             (data_array_orig[index * size + 41]), (data_array_orig[index * size + 40]), 
             (data_array_orig[index * size + 39]), (data_array_orig[index * size + 38]),
             (data_array_orig[index * size + 37]), (data_array_orig[index * size + 36]),
             (data_array_orig[index * size + 35]), (data_array_orig[index * size + 34]), 
             (data_array_orig[index * size + 33]), (data_array_orig[index * size + 32]),
             (data_array_orig[index * size + 31]), (data_array_orig[index * size + 30]),
             (data_array_orig[index * size + 29]), (data_array_orig[index * size + 28]), 
             (data_array_orig[index * size + 27]), (data_array_orig[index * size + 26]),
             (data_array_orig[index * size + 25]), (data_array_orig[index * size + 24]),
             (data_array_orig[index * size + 23]), (data_array_orig[index * size + 22]), 
             (data_array_orig[index * size + 21]), (data_array_orig[index * size + 20]),
             (data_array_orig[index * size + 19]), (data_array_orig[index * size + 18]),
             (data_array_orig[index * size + 17]), (data_array_orig[index * size + 16]), 
             (data_array_orig[index * size + 15]), (data_array_orig[index * size + 14]),
             (data_array_orig[index * size + 13]), (data_array_orig[index * size + 12]),
             (data_array_orig[index * size + 11]), (data_array_orig[index * size + 10]), 
             (data_array_orig[index * size + 9]), (data_array_orig[index * size + 8]),
             (data_array_orig[index * size + 7]), (data_array_orig[index * size + 6]),
             (data_array_orig[index * size + 5]), (data_array_orig[index * size + 4]), 
             (data_array_orig[index * size + 3]), (data_array_orig[index * size + 2]),
             (data_array_orig[index * size + 1]), (data_array_orig[index * size])};
}
/**
 * @param index tag of the cache line
 * @return Line structured data_array entry
 */
Line cTol(int index){
    return { (data_array[index * size + 63]), (data_array[index * size + 62]),
             (data_array[index * size + 61]), (data_array[index * size + 60]),
             (data_array[index * size + 59]), (data_array[index * size + 58]), 
             (data_array[index * size + 57]), (data_array[index * size + 56]),
             (data_array[index * size + 55]), (data_array[index * size + 54]),
             (data_array[index * size + 53]), (data_array[index * size + 52]), 
             (data_array[index * size + 51]), (data_array[index * size + 50]),
             (data_array[index * size + 49]), (data_array[index * size + 48]),
             (data_array[index * size + 47]), (data_array[index * size + 46]), 
             (data_array[index * size + 45]), (data_array[index * size + 44]),
             (data_array[index * size + 43]), (data_array[index * size + 42]),
             (data_array[index * size + 41]), (data_array[index * size + 40]), 
             (data_array[index * size + 39]), (data_array[index * size + 38]),
             (data_array[index * size + 37]), (data_array[index * size + 36]),
             (data_array[index * size + 35]), (data_array[index * size + 34]), 
             (data_array[index * size + 33]), (data_array[index * size + 32]),
             (data_array[index * size + 31]), (data_array[index * size + 30]),
             (data_array[index * size + 29]), (data_array[index * size + 28]), 
             (data_array[index * size + 27]), (data_array[index * size + 26]),
             (data_array[index * size + 25]), (data_array[index * size + 24]),
             (data_array[index * size + 23]), (data_array[index * size + 22]), 
             (data_array[index * size + 21]), (data_array[index * size + 20]),
             (data_array[index * size + 19]), (data_array[index * size + 18]),
             (data_array[index * size + 17]), (data_array[index * size + 16]), 
             (data_array[index * size + 15]), (data_array[index * size + 14]),
             (data_array[index * size + 13]), (data_array[index * size + 12]),
             (data_array[index * size + 11]), (data_array[index * size + 10]), 
             (data_array[index * size + 9]), (data_array[index * size + 8]),
             (data_array[index * size + 7]), (data_array[index * size + 6]),
             (data_array[index * size + 5]), (data_array[index * size + 4]), 
             (data_array[index * size + 3]), (data_array[index * size + 2]),
             (data_array[index * size + 1]), (data_array[index * size])};
}
/**
 * @param index tag of the cache line
 * @return Line structured data_array_perf entry
 */
Line cTol_perf(int index){
    return { (data_array_perf[index * size + 63]), (data_array_perf[index * size + 62]),
             (data_array_perf[index * size + 61]), (data_array_perf[index * size + 60]),
             (data_array_perf[index * size + 59]), (data_array_perf[index * size + 58]), 
             (data_array_perf[index * size + 57]), (data_array_perf[index * size + 56]),
             (data_array_perf[index * size + 55]), (data_array_perf[index * size + 54]),
             (data_array_perf[index * size + 53]), (data_array_perf[index * size + 52]), 
             (data_array_perf[index * size + 51]), (data_array_perf[index * size + 50]),
             (data_array_perf[index * size + 49]), (data_array_perf[index * size + 48]),
             (data_array_perf[index * size + 47]), (data_array_perf[index * size + 46]), 
             (data_array_perf[index * size + 45]), (data_array_perf[index * size + 44]),
             (data_array_perf[index * size + 43]), (data_array_perf[index * size + 42]),
             (data_array_perf[index * size + 41]), (data_array_perf[index * size + 40]), 
             (data_array_perf[index * size + 39]), (data_array_perf[index * size + 38]),
             (data_array_perf[index * size + 37]), (data_array_perf[index * size + 36]),
             (data_array_perf[index * size + 35]), (data_array_perf[index * size + 34]), 
             (data_array_perf[index * size + 33]), (data_array_perf[index * size + 32]),
             (data_array_perf[index * size + 31]), (data_array_perf[index * size + 30]),
             (data_array_perf[index * size + 29]), (data_array_perf[index * size + 28]), 
             (data_array_perf[index * size + 27]), (data_array_perf[index * size + 26]),
             (data_array_perf[index * size + 25]), (data_array_perf[index * size + 24]),
             (data_array_perf[index * size + 23]), (data_array_perf[index * size + 22]), 
             (data_array_perf[index * size + 21]), (data_array_perf[index * size + 20]),
             (data_array_perf[index * size + 19]), (data_array_perf[index * size + 18]),
             (data_array_perf[index * size + 17]), (data_array_perf[index * size + 16]), 
             (data_array_perf[index * size + 15]), (data_array_perf[index * size + 14]),
             (data_array_perf[index * size + 13]), (data_array_perf[index * size + 12]),
             (data_array_perf[index * size + 11]), (data_array_perf[index * size + 10]), 
             (data_array_perf[index * size + 9]), (data_array_perf[index * size + 8]),
             (data_array_perf[index * size + 7]), (data_array_perf[index * size + 6]),
             (data_array_perf[index * size + 5]), (data_array_perf[index * size + 4]), 
             (data_array_perf[index * size + 3]), (data_array_perf[index * size + 2]),
             (data_array_perf[index * size + 1]), (data_array_perf[index * size])};
}
/**
 * @param index tag of the cache line
 * @return Line structured data_array_sub entry
 */

long long unsigned * convertBuffer2Array (long long unsigned* values, char * buffer, unsigned s, unsigned step)
{
      /* Don't malloc! -nzb */

      /* long long unsigned * values = (long long unsigned *) VG_(malloc)("cg.compress.ci.1", sizeof(long long unsigned) * size/step); */
//      std::cout << std::dec << "ConvertBuffer = " << size/step << std::endl;
     //init
    unsigned int i, j;
     for (i = 0; i < s / step; i++) {
          values[i] = 0;    // Initialize all elements to zero.
      }
      //SIM_printf("Element Size = %d \n", step);
      for (i = 0; i < s; i += step ){
          for (j = 0; j < step; j++){
              //SIM_printf("Buffer = %02x \n", (unsigned char) buffer[i + j]);
              values[i / step] += (long long unsigned)((unsigned char)buffer[i + j]) << (8*j);
              //long long unsigned val = ((unsigned char)buffer[i + j]) << (8*j);
              //values[i / step] += val;
              //if (j >= 4 && buffer[i + j] !=0 && val < 1llu << 32 ) {
              //    info("j %u buffer[i + j] %u val %llu", j, (unsigned char) buffer[i + j], val);
              //    //panic("Damn it nzb");
              //  }
             //SIM_printf("step %d value = ", j);
              //printLLwithSize(values[i / step], step);  
          }
          //std::cout << "Current value = " << values[i / step] << std::endl;
          //printLLwithSize(values[i / step], step);
          //SIM_printf("\n");
      }
      //std::cout << "End ConvertBuffer = " << size/step << std::endl;
      return values;
}

int isZeroPackable ( long long unsigned * values, unsigned s){
  int nonZero = 0;
  unsigned int i;
  for (i = 0; i < s; i++) {
      if( values[i] != 0){
          nonZero = 1;
          break;
      }
  }
  return !nonZero;
}

///
/// Check if the cache line consists of only same values
///
int isSameValuePackable ( long long unsigned * values, unsigned s){
  int notSame = 0;
  unsigned int i;
  for (i = 0; i < s; i++) {
      if( values[0] != values[i]){
          notSame = 1;
          break;
      }
  }
  return !notSame;
}

unsigned multBaseCompression ( long long unsigned * values, unsigned s, unsigned blimit, unsigned bsize){
  unsigned long long limit = 0;
  unsigned BASES = 1;
  //define the appropriate size for the mask
  switch(blimit){
    case 1:
      limit = 0xFF;
      break;
    case 2:
      limit = 0xFFFF;
      break;
    case 4:
      limit = 0xFFFFFFFF;
      break;
    default:
      //std::cout << "Wrong blimit value = " <<  blimit << std::endl;
      VG_(exit)(1);
  }
  // finding bases: # BASES
  //std::vector<unsigned long long> mbases;
  //mbases.push_back(values[0]); //add the first base
  unsigned long long mbases [64];
  unsigned baseCount = 1;
  mbases[0] = 0;
  unsigned int i,j;
  for (i = 0; i < s; i++) {
      for(j = 0; j <  baseCount; j++){
         if( my_llabs((long long int)(mbases[j] -  values[i])) > limit ){
             //mbases.push_back(values[i]); // add new base
             mbases[baseCount++] = values[i];
         }
     }
     if(baseCount >= BASES) //we don't have more bases
       break;
  }
  // find how many elements can be compressed with mbases
  unsigned compCount = 0;
  for (i = 0; i < s; i++) {
      //ol covered = 0;
      for(j = 0; j <  baseCount; j++){
         if( my_llabs((long long int)(mbases[j] -  values[i])) <= limit ){
             compCount++;
             break;
         }
     }
  }
  //return compressed size
  unsigned mCompSize = blimit * compCount + bsize * BASES + (s - compCount) * bsize;
  if(compCount < s)
     return s * bsize;
  //VG_(printf)("%d-bases bsize = %d osize = %d CompCount = %d CompSize = %d\n", BASES, bsize, blimit, compCount, mCompSize);
  //printf("%d-bases bsize = %d osize = %d CompCount = %d CompSize = %d\n", BASES, bsize, blimit, compCount, mCompSize);

  return mCompSize;
}

void axorb(char* c, char* a, char* b)
{
    for(int i=0;i<64;i++)
	c[i] = a[i] ^ b[i];
}

unsigned BDICompress (char * buffer, unsigned _blockSize)
{
  //char * dst = new char [_blockSize];
//  print_value(buffer, _blockSize);

    long long unsigned array[ _blockSize * 8 ];

  long long unsigned * values = convertBuffer2Array( array, buffer, _blockSize, 8);
  unsigned bestCSize = _blockSize;
  unsigned currCSize = _blockSize;
  if( isZeroPackable( values, _blockSize / 8))
      bestCSize = 1;
  if( isSameValuePackable( values, _blockSize / 8))
      currCSize = 8;
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  currCSize = multBaseCompression( values, _blockSize / 8, 1, 8);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  currCSize = multBaseCompression( values, _blockSize / 8, 2, 8);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  currCSize =  multBaseCompression( values, _blockSize / 8, 4, 8);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  // VG_(free)(values);
  values = convertBuffer2Array( array, buffer, _blockSize, 4);
  if( isSameValuePackable( values, _blockSize / 4))
      currCSize = 4;
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  currCSize = multBaseCompression( values, _blockSize / 4, 1, 4);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  currCSize = multBaseCompression( values, _blockSize / 4, 2, 4);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  // VG_(free)(values);
  values = convertBuffer2Array( array, buffer, _blockSize, 2);
  currCSize = multBaseCompression( values, _blockSize / 2, 1, 2);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  // VG_(free)(values);

  //exponent base compression
  /*values = convertBuffer2Array( array, buffer, _blockSize, 8);
  currCSize = doubleExponentCompression( values, _blockSize / 8, 2, 8);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  currCSize = doubleExponentCompression( values, _blockSize / 8, 1, 8);
  bestCSize = bestCSize > currCSize ? currCSize: bestCSize;
  VG_(free)(values);*/

  //delete [] buffer;
  buffer = NULL;
  values = NULL;
  //SIM_printf(" BestCSize = %d \n", bestCSize);
  return bestCSize;

}

Line cTol_sub(int index){
    return { (data_array_sub[index * size + 63]), (data_array_sub[index * size + 62]),
             (data_array_sub[index * size + 61]), (data_array_sub[index * size + 60]),
             (data_array_sub[index * size + 59]), (data_array_sub[index * size + 58]), 
             (data_array_sub[index * size + 57]), (data_array_sub[index * size + 56]),
             (data_array_sub[index * size + 55]), (data_array_sub[index * size + 54]),
             (data_array_sub[index * size + 53]), (data_array_sub[index * size + 52]), 
             (data_array_sub[index * size + 51]), (data_array_sub[index * size + 50]),
             (data_array_sub[index * size + 49]), (data_array_sub[index * size + 48]),
             (data_array_sub[index * size + 47]), (data_array_sub[index * size + 46]), 
             (data_array_sub[index * size + 45]), (data_array_sub[index * size + 44]),
             (data_array_sub[index * size + 43]), (data_array_sub[index * size + 42]),
             (data_array_sub[index * size + 41]), (data_array_sub[index * size + 40]), 
             (data_array_sub[index * size + 39]), (data_array_sub[index * size + 38]),
             (data_array_sub[index * size + 37]), (data_array_sub[index * size + 36]),
             (data_array_sub[index * size + 35]), (data_array_sub[index * size + 34]), 
             (data_array_sub[index * size + 33]), (data_array_sub[index * size + 32]),
             (data_array_sub[index * size + 31]), (data_array_sub[index * size + 30]),
             (data_array_sub[index * size + 29]), (data_array_sub[index * size + 28]), 
             (data_array_sub[index * size + 27]), (data_array_sub[index * size + 26]),
             (data_array_sub[index * size + 25]), (data_array_sub[index * size + 24]),
             (data_array_sub[index * size + 23]), (data_array_sub[index * size + 22]), 
             (data_array_sub[index * size + 21]), (data_array_sub[index * size + 20]),
             (data_array_sub[index * size + 19]), (data_array_sub[index * size + 18]),
             (data_array_sub[index * size + 17]), (data_array_sub[index * size + 16]), 
             (data_array_sub[index * size + 15]), (data_array_sub[index * size + 14]),
             (data_array_sub[index * size + 13]), (data_array_sub[index * size + 12]),
             (data_array_sub[index * size + 11]), (data_array_sub[index * size + 10]), 
             (data_array_sub[index * size + 9]), (data_array_sub[index * size + 8]),
             (data_array_sub[index * size + 7]), (data_array_sub[index * size + 6]),
             (data_array_sub[index * size + 5]), (data_array_sub[index * size + 4]), 
             (data_array_sub[index * size + 3]), (data_array_sub[index * size + 2]),
             (data_array_sub[index * size + 1]), (data_array_sub[index * size])};
}
/**
 * 
 * @param index tag of the cache line
 * @return Line structured data_array entry
 */
void LongToLine (Line line, uint32_t tag){
    sizes[tag][0] = countLine(line);
	//printf("size of line: %d\n", sizes[tag][0]);
    for(int i = 0; i < size; i++){
        data_array_orig[tag * size + i] = (unsigned char) line.Byte[i];
    }
}
int countLine(Line line){
	//printf("\n");
    int ret = 0;
    for(int i = 0; i < 64; i++){
		//printf("%d %u as width %d\n", i, (unsigned int) line.Byte[i], countWidth((unsigned short int)line.Byte[i]));
        ret += (countWidth((unsigned short int)line.Byte[i]));
    }
    if(ret % 8 > 0)
        return (ret/8) + 1;
    else
        return ret/8;
}
/**
 * count the number of bits int a byte
 * @param num byte to count
 * @return number of bits that are not padding zeroes
 */
int countWidth(unsigned short int num){
    for(int i = 0; i < 8; i++){
        if(num <= (short)pow(2,i)-1)
            return i;
    }
    return 8;
}
/**
 * count the number of bits int a byte
 * @param input Line to count
 * @return number of bits that are not continuous
 */
int countFlip(Line input){
    int ret = 0;
    Line line = input;
    int mask = 1;
    int last = mask & (int)line.Byte[0];
    for(int i = 0; i < 64;i++){
      for(int i = 0; i < 31; i++){
          if(last != (int)(1 & line.Byte[i])){
              ret ++;
              last = (int)(1 & line.Byte[i]);
          }
          line.Byte[i] = line.Byte[i] >> 1;
      }
    }
    return ret;
}
/**
 * subtract two lines
 * @param l1 
 * @param l2 
 * @return result
 */
Line sub(Line l1, Line l2){ 
    Line result = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
    for(int i = 0 ; i < size; i++){
        result.Byte[i] = l1.Byte[i] - l2.Byte[i];
    }
    return result;
}
/**
 * add two lines
 * @param l1 
 * @param l2 
 * @return result
 */
Line add(Line l1, Line l2){ 
    Line result = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
    for(int i = 0 ; i < size; i++){
        result.Byte[i] = l1.Byte[i] + l2.Byte[i];
    }
    return result;
}
/**
 * xor two lines
 * @param l1 
 * @param l2 
 * @return result
 */
Line xxor(Line l1, Line l2){ 
    Line result = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
    for(int i = 0 ; i < size; i++){
        result.Byte[i] = l1.Byte[i] ^ l2.Byte[i];
    }
    return result;
}
/**
 * print a line
 */
void printline(Line line){
	for(int i = 0 ; i < 64; i++){
		//printf("%u |", line.Byte[i]);	
	}
	//printf("\n");
}
/**
 * check if a line is empty
 */
int Empty(Line line){
  for(int i = 0 ; i < 64; i ++){
    if(line.Byte[i] != 0)
      return 1;
  }
  return 0;
}
/**
 * iterate through original array and test the compressibilty of the current cache
 *
 * @return the maximum compression rate 
 */
double best_compression_rate(){
  int ret = 0;
  int orig = 0;
  // iterate through
  for(int i = 0 ; i < 32768; i++){
    if(initialized[i][0] == 1){
      Line line = cTol_orig(i);
      int length = countLine(line);
      orig += length;
      for(int j = 0 ; j < 32768; j ++){
        if(i != j && countLine(xxor(cTol_orig(j), line)) < length ){
          length = countLine(xxor(cTol_orig(j), line));
        }
      }
      ret += length;
    }
  }
  return (double)ret/(double)orig;
}
/**
 * iterate through perfect array and test the compressibilty of the current cache
 *
 * @param data_array_perf perfect data_array
 * @return the maximum compression rate 
 */
double best_possible_rate(){
  int ret = 0;
  int orig = 0;
  // iterate through
  for(int i = 0 ; i < 32768; i++){
    if(initialized[i][0] == 1){
      Line line = cTol_orig(i);
      orig += countLine(line);
      ret += countLine(cTol_perf(i));
    }
  }
  return (double)ret/(double)orig;
}
////////////////////////////////////////////NOT IN USE//////////////////////////////////////////
int getTag(long long line){
    return (int)((line & 0x0000007f));
}
