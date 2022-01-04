/* 
 * CS:APP Data Lab 
 * 
 * Lewis Arnsten, CNETID: larnsten
 * 
 * bits.c - Source file with your solutions to the project.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the project by
editing the collection of functions in this source file.

CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according to the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest checker to verify that your solutions produce
 *      the correct answers.
 */


#endif

/************************************************ 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x){
  /* A mask is created of all 1 bits, which is added to x
  so xor will then return the magnitude of the input. */
  int mask = x >> 31;
  int final = x + mask;
  final = final ^ mask;

  return final;
}

/************************************************ 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  /* First the sign bits of x, y, and the sum of x and y are retrieved. 
  Use xor to check if x and y have different signs. If they are the same,
  check that the sign of x is different from z and y is different from z. */
  int z = (x+y) >> 31;
  x = x >> 31;
  y = y >> 31;

  return !!(x^y) | (!(x^z) & !(y^z));
}

/************************************************ 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  /* The intial 32 bit integer contains all even bits of 1 if it is a 
  repeated 2 bit sequence. So the first half of the 32 bit integer is compared to
  the second half to see if they have the same sequence (determined with &).
  Then that is repeated with each smaller half until it is just comparing 1 bit. */
  x = x & (x >> 16);
  x = x & (x >> 8);
  x = x & (x >> 4);
  x = x & (x >> 2);

  return x & 1;
}

/************************************************
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /* The bitwise complement of x is summed with 1 to get the negative of x. 
  Then if x is not zero the sign bit of neg_x or x will be 1. Added to 1 to 
  replicate functionality of !. */
  int neg_x = ~x+1;
  int signbit = (neg_x | x) >> 31;

  return signbit + 1;
}

/************************************************
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /* First the number of 1's is counted in the first four bits. Then the mask 
  is readjusted to perserve the count and continue onto the next byte. 
  The new mask is used to continue adding 4 bits. */
  int init_mask = 0x11 | (0x11 << 8);
  int mask = init_mask | (init_mask << 16);
  int new_mask = 0xF | (0xF << 8);
  int count = x & mask;
  count = count + ((x >> 1) & mask);
  count = count + ((x >> 2) & mask);
  count = count + ((x >> 3) & mask);
  count = count + (count >> 16);
  count = (count & new_mask) + ((count >> 4) & new_mask);

  return ((count + (count >> 8)) & 0x3F);
}

/************************************************
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  /* Nor returns 1 when both inputs are 0. */
  return (~x) & (~y);
}

/************************************************
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  /* First each byte is retrieved by shifting left 3. Then 0xFF is left
  shifted by n Or m and the inverse is taken. X is then right shifted by m and n 
  and & with 0xFF to reutrn the new bytes. */
  int n2 = n << 3;
  int m2 = m << 3;
  int nOrM = ~((0xFF << n2) | (0xFF << m2));
  int first = (x >> n2) & 0xFF;
  int second = (x >> m2) & 0xFF;
  
  return (x & nOrM) | ((second << n2) | (first << m2));
}

/************************************************ 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* A mask is created depending on if x is 0 or non-zero. 
  This mask is used with z, and y if that returned 0. */
  int mask = (!x + ~0);

  return (~mask & z) | (mask & y);
}

/************************************************
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
  /* First x is multiplied by 3. Then the sign of x is found.
  Then the sign is applied and x is sifted right 2 to divide by 4. */
  int x3 = x+x+x;  
  int sign = x3>>31;  

  return (x3 + (sign & 3)) >> 2;
}

/************************************************ 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  /* Need to determine if the first 32 - n bits are equal. 
  All high order bits are changed to a 1 or 0. Then the shift
  is reversed and xor is used to see if the number changed. */
  int shift = ~n + 33;
  int shifted = x << shift;
  int back = shifted >> shift;

  return !(back ^ x);
}

/************************************************
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /* N is shifted left by 3 or multiplied by 8. The nth byte is then
  shifted to the rightmost position and extracted using 0xFF. */
  int shift = n << 3; 
  int final = x >> shift; 

  return final & 0xff; 
}

/************************************************ 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
  /* X is continually shifted to the right, turning neighboring bits to 1. 
  Then invert the number, shift right 1, and use & to isolate the most
  significant 1 bit. */
  x = x | x >> 1;
  x = x | x >> 2;
  x = x | x >> 4;
  x = x | x >> 8;
  x = x | x >> 16;

  return x & ((~x >> 1) ^ (1 << 31));
}

/************************************************
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
  /* Returns 1 if y is 1 or if x and y are the same value. */
  return (!x) | y;
}

/************************************************ 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /* First the upper and lower bounds of x are determined using the sign.
  Then x is added to the upper and lower bounds, and the sign bit is checked.
  Both results must be positive for the digit to be in 0x30 <= x <= 0x39. */
  int sign = 1 << 31;
  int upper = ~(sign | 0x39); 
  int lower = ~0x30;
  upper = sign & (upper + x) >> 31;
  lower = sign & (lower + x + 1) >> 31; 

  return !(upper | lower); 
}

/************************************************ 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  /* Xor will return 0 when two bits are the same and 1 otherwise. Thus if xor
  returns 0, then the two numbers are equal and ! produces the correct output. */
  return !(x ^ y);
}

/************************************************
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
  /* X is less than y if x < 0 and y >= 0 or if 
  the sign of x = sign of y and x - y < 0. 
  The sign (int sign) is set to 1 if and only if x is negative and y is positive.
  The sign of the difference (difference) is also found. The final result is 1 
  if x < y, the difference is positive, and x and y have the same sign
  or if x is negative and y is positive.*/
  int difference = y + (~x);
  int sign = ((x ^ y) >> 31 ) & 1;
  int final = ~sign;
  sign = sign & (( x >> 31 ) & 1);
  difference = ((~difference >> 31 ) & 1);
  
  return (final & difference) | sign; 
}

/************************************************
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  /* If the sign bit is 1, 0 is returned. Otherwise 1 is returned. */
  x = x >> 31; 

  return !x;
}

/************************************************
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
  /* Every power of 2 has only one bit set to 1, and every power of 
  two minus one is a string of ones. So here the first bit is retrieved
  and the number minus 1 is tested.*/
  int sign = !(x >> 31);
  int MinusOne = !((x + ~0) & x);

  return (!!x) & sign & MinusOne;
}

/************************************************
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
    /* Checks if x and its inverse are equal. Also
    makes sure that x is not equal to 0. */
  return !((x ^ (~x + 1)) + !x);
}

/************************************************
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  /* -1 is the inverse of 0. */
  return ~0;
}

/************************************************
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  /* First the distance to shift is found. Then a mask is created 
  for 1s after the right shift. The bits to save are then found by shifting
  X right and the rotated bytes are extracted by shifting left.*/
  int shift_mask = 32 + ~n;
  int end_mask = ~(~0 << n);
  int saved = (x >> shift_mask) >> 1;
  saved = end_mask & saved;
  x = x << n;

  return x|saved;
}

/************************************************
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
  /* When multiplying, if the sign changes, then all bits should be 1. If the sign
  does not change all bits should be 0. A mask for tmax is used if x >= 0 and a mask for
  tmin us used if x < 0. */
  int overflow = (x^(x + x)) >> 31; 
  int maxOrmin = overflow & ((x>>31) ^ ~(0x1<<31)); 

  return (maxOrmin) | (~overflow & (x + x));
}

