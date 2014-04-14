// fixpt.h -- fixed point operations
//  These are reasonably efficient, especially compared to floating point.
//  For consistency, all functions begin with the number of whole bits
//  and fraction bits, although these are not always actually used.
//  For unsigned fixed point:
//  -- NFractionBits determines the scaling, i.e. for NFractionBits==4,
//      a value of 1.5 is stored as (1<<4)+(1<<3) or 0x18 .
//  -- NWholeBits are some or all of the other bits higher in the word,
//      and is used for saturation (limiting to within a range);
//      potentially a debug version of this might actually enforce
//      the maximum range at all steps of the process.
//
//  Note that for signed fixed point, a bit is always used as the
//  sign bit so NFractionBits+NWholeBits must be less than the number
//  of bits in a word (32 for most bigger embedded architectures).
//
//  NOTE: This code does NOT support (in general) negative values
//  for NWholeBits or for NFractionBits (which would be associated
//  with the "point" lying out of the range of the stored bits).
//
//  Here is a diagram, with most significant bit of word on the left:
//      [sign bit][guard bits][whole bits][fraction bits]
//  -- The sign bit is implicitly allocated for signed fixed point, and
//      not present for unsigned.
//  -- Guard bits are good because the allow intermediates to exceed
//      the normal range without loss of precision.
//  -- Whole bits (if present) allow representation of integer values.
//  -- Fractional bits (if present) allow represenation of fractional values.
//
//  It is assumed that right shift of signed numbers extends sign bit
//  (true of compilers for most modern embedded processors; if 
//  processor has an "arithmetic shift right" then compiler wll use it).
//
//  2's complement arithmetic (assumed) has the odd property that there
//  are more negative numbers than positive numbers; the most negative
//  number representable with a given number of bits has no positive
//  inverse (it is it's own inverse, something like "infinity").
//  Our approach is to try to avoid such most negative numbers as much
//  as we can; for saturation/limiting to n bits we restrict the range
//  to +/-((1<<n)-1).
//      
//
//  BEWARE OF:
//  -- Overflow is possible at many stages and is undetected.
//  -- Loss of precision can be extreme.
//  -- For a particular use, you must track the number of fraction bits
//      you are using and pass to each function.
//  -- The number of fraction bits you use should normally be in the
//      range of 10 to 15 depending on the application;
//      certainly never larger than 31 for signed or 32 for unsigned;
//      if fixed point results are to be stored in 16 bit locations,
//      then 15 or 16 respectively.
//  -- For efficiency, the number of fraction bits should be 
//      known by the compiler as a constant.
//
//  HINTS:
//  -- For mixed mode arithmetic (signed and unsigned), ensure
//      that all your unsigned numbers can actually be represented as
//      signed, and do everyting as signed.
//
//  DEBUGGING:
//  -- Someone could write a non-efficient version which did strict
//      checking at all stages and flagged errors.
#ifndef fixpt__h
#define fixpt__h        // once only

// ufixpt, sfixpt -- unsigned and signed fixed point types.
//      Upper bits are integer part, lower are fractional.
//      Exactly how many bits for each must be provided for each call
//      where this information is needed.
typedef unsigned ufixpt;
typedef signed sfixpt;
#define fixptNBitsPerInt 32           // word size! FIX: from somewhere else
typedef unsigned long long ufixptLong;
typedef signed long long sfixptLong;


// divide by power of 2 with rounding.
//      In case of a tie, this always rounds consistently
//      (up in current implementation).
//      Valid only for power of 2 >= 0.
static inline ufixpt ufixptRightShiftRounded(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    unsigned shift           // the power of 2, must be >= 0
    )
{
    if ( shift > 0 ) return ((x >> (shift-1))+1)>>1;
    return x;
}

// divide by power of 2 with rounding.
//      In case of a tie, this always rounds consistently
//      (up in current implementation).
//      Valid only for power of 2 >= 0.
static inline sfixpt sfixptRightShiftRounded(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    unsigned shift           // the power of 2, must be >= 0
    )
{
    // Assume sign bit propogates
    if ( shift > 0 ) return ((x >> (shift-1))+1)>>1;
    return x;
}

// divide 64bit fixpt by power of 2 with rounding.
//      In case of a tie, this always rounds consistently
//      (up in current implementation).
//      Valid only for power of 2  >= 0.
static inline ufixptLong ufixptRightShiftRoundedLong(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixptLong x,
    unsigned shift           // the power of 2, must be >= 0
    )
{
    if ( shift > 0 ) return ((x >> (shift-1))+1)>>1;
    return x;
}

// divide 64bit fixpt by power of 2 with rounding.
//      In case of a tie, this always rounds consistently
//      (up in current implementation).
//      Valid only for power of 2  >= 0.
static inline sfixptLong sfixptRightShiftRoundedLong(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixptLong x,
    unsigned shift           // the power of 2, must be >= 0
    )
{
    // Assume sign bit propogates
    if ( shift > 0 ) return ((x >> (shift-1))+1)>>1;
    return x;
}


static inline ufixpt ufixptFromInt(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    unsigned x )
{
    (void) NWholeBits;
    return x << NFractionBits;
}

static inline sfixpt sfixptFromInt(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    unsigned x)
{
    (void) NWholeBits;
    return x << NFractionBits;
}

static inline sfixpt sfixptFromTimeval(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    unsigned sec, unsigned usec)
{
    (void) NWholeBits;
    return (sec << NFractionBits) + (sfixpt)(((sfixptLong)usec << NFractionBits)/1000000);
}


static inline unsigned ufixptToInt(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    ufixpt x)
{
    (void) NWholeBits;
    return x >> NFractionBits;
}

static inline signed sfixptToInt(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    sfixpt x)
{
    (void) NWholeBits;
    return x >> NFractionBits;
}

static inline void sfixptToTimeval(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    sfixpt x, 
    signed *sec, signed *usec)
{
    (void) NWholeBits;
    *sec = x >> NFractionBits;
    *usec = (((sfixptLong)(x-(*sec<<NFractionBits)) * 1000000) >> NFractionBits);
}

static inline unsigned ufixptToIntRounded(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    ufixpt x)
{
    return ufixptRightShiftRounded( NWholeBits, NFractionBits,
        x,
        NFractionBits
        );
}

static inline signed sfixptToIntRounded(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    sfixpt x)
{
    (void) NWholeBits;
    return x >> NFractionBits;
}

static inline ufixpt ufixptFromFloat(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    double x)
{
    (void) NWholeBits;
    return (unsigned)(x * (1<<NFractionBits));
}

static inline sfixpt sfixptFromFloat(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    double x)
{
    (void) NWholeBits;
    return (signed)(x * (1<<NFractionBits));
}

static inline double ufixptToFloat(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    ufixpt x)
{
    (void) NWholeBits;
    return ((double)x)/(1<<NFractionBits);
}

static inline double sfixptToFloat(
    unsigned NWholeBits,
    unsigned NFractionBits, 
    sfixpt x)
{
    (void) NWholeBits;
    return ((double)x)/(1<<NFractionBits);
}

// addition/subtraction -- simply add or subtract directly
// or call these functions if you want to be consistent.
// Due to 2's complement arithmetic, the result is same regardless
// of signedness of either operand.
static inline ufixpt ufixptAdd(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    ufixpt y )
{
    (void) NWholeBits;
    (void) NFractionBits;
    return x+y;
}

// addition/subtraction -- simply add or subtract directly
// or call these functions if you want to be consistent
// Due to 2's complement arithmetic, the result is same regardless
// of signedness of either operand.
static inline sfixpt sfixptAdd(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    sfixpt y )
{
    (void) NWholeBits;
    (void) NFractionBits;
    return x+y;
}

// saturation -- clip overly positive to largest allowed.
// This is usually better than wrapping!
//
static inline ufixpt ufixptSaturate(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x )
{
    unsigned nbits = NFractionBits + NWholeBits;
    // so that we can handle e.g. 0.32 fixpt, we have the following
    // convoluted assignment to maxvalue,
    // otherwise this could be just:  ((1<<nbits)-1)
    unsigned maxvalue = ((1U<<(nbits-1))-1)+(1U<<(nbits-1));
    // Saturate properly, avoiding overflow
    if ( x > maxvalue ) x = maxvalue;
    return x;
}


// saturation -- clip overly negative numbers to smallest allowed
// and overly positive to largest allowed.
// This is usually better than wrapping!
// The sum of NFractionBits and NWholeBits must not exceed 31 because
// there must always be a sign bit.
//
// Example: NFractionBits=30, NWholeBits=0, note 1 bit is used for sign bit;
//      The real number value is kept in the range (-1,1) exclusive
//      i.e. stored as between [-0x3FFFFFFF, 0x3FFFFFFF] inclusive.
static inline sfixpt sfixptSaturate(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x )
{
    // This algorithm converts to assembler better than it looks...
    // at least for mips...
    // i've tried some other algorithms that looked better 
    // (or at least better in the typical not-saturated case)
    // and they were actually worse.  -TM
    unsigned nbits = NFractionBits + NWholeBits;
    signed maxvalue = (1U<<nbits)-1;
    signed minvalue = -maxvalue;
    // Saturate properly, avoiding overflow
    if ( x < minvalue ) x = minvalue;
    else
    if ( x > maxvalue ) x = maxvalue;
    return x;
}


// addition with saturation to avoid wraparound.
// The sum of NFractionBits and NWholeBits must not exceed 31 because
// there must always be a guard bit and a sign bit
// and because of details of ufixptSaturate.
//
// Example: NFractionBits=31, NWholeBits=0,
// 31 bits of a word are in use, leaving one as a guard.
//      The real number value is kept in the range (-1,1) exclusive
//      i.e. stored as between [-0x7FFFFFFF, 0x7FFFFFFF] inclusive.
static inline ufixpt ufixptAddSaturate(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    ufixpt y )
{
    ufixpt sum = x+y;
    return ufixptSaturate(NFractionBits, NWholeBits, sum);
}

// addition/subtraction with saturation to avoid wraparound.
// The sum of NFractionBits and NWholeBits must not exceed 30 because
// there must always be a guard bit and a sign bit.
//
// Example: NFractionBits=30, NWholeBits=0, note 1 bit is used for sign bit;
// 31 bits of a word are in use, leaving one as a guard.
//      The real number value is kept in the range (-1,1) exclusive
//      i.e. stored as between [-0x3FFFFFFF, 0x3FFFFFFF] inclusive.
static inline sfixpt sfixptAddSaturate(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    sfixpt y )
{
    sfixpt sum = x+y;
    return sfixptSaturate(NFractionBits, NWholeBits, sum);
}


// fast multiplication:
// (this is appropriate only when the sum of the bits actually used
// is less than word size).
static inline ufixpt ufixptSmallMult(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    ufixpt y )
{
    unsigned temp = x*y;
    return (temp >> NFractionBits);
}


// fast multiplication:
// (this is appropriate only when the sum of the bits actually used
// is less than word size).
static inline sfixpt sfixptSmallMult(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    sfixpt y )
{
    signed temp = x*y;
    return (temp >> NFractionBits);
}


// multiplication:
// (this uses long long intermediate to reduce chance of overflow)
static inline ufixptLong ufixptMult(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    ufixpt y )
{
    unsigned long long full = (unsigned long long)x*y;
    return (full >> NFractionBits);
}

// multiplication:
// (this uses long long intermediate to reduce chance of overflow)
static inline sfixptLong sfixptMult(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    sfixpt y )
{
    signed long long full = (signed long long)x*y;
    return (full >> NFractionBits);
}

// multiplication by integer (small operands):
//      (as it happens, the argument order doesn't matter)
// (this is appropriate only when the sum of the bits actually used
// is less than word size).
static inline ufixpt ufixptMultByIntSmall(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    unsigned y )
{
    return x*y;
}

// multiplication by integer (small operands):
//      (as it happens, the argument order doesn't matter)
// (this is appropriate only when the sum of the bits actually used
// is less than word size).
static inline sfixpt sfixptMultByIntSmall(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    signed y )
{
    return x*y;
}

// multiplication by integer:
//      (as it happens, the argument order doesn't matter)
static inline ufixptLong ufixptMultByInt(
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    unsigned y )
{
    (void) NWholeBits;
    (void) NFractionBits;
    unsigned long long full = (unsigned long long)x*y; 
    return full;
}

// multiplication by integer:
//      (as it happens, the argument order doesn't matter)
static inline sfixptLong sfixptMultByInt(
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    signed y )
{
    (void) NWholeBits;
    (void) NFractionBits;
    signed long long full = (signed long long)x*y;
    return full;
}

// inverse of unsigned integer x as fixpt number (small case)
//      SMALL means that NFractionBits is <= 31,
//      i.e. this is NOT suitable if you want to use entire wordwidth
//      with no whole bits (0.32 format).
//      The special case of x==0 is explicitly handled --> saturation.
//      This works well for smaller values of x,
//      losing precision as x gets larger and zeroing out for large x.
static inline ufixpt ufixptInverseOfUIntSmall(
    unsigned NWholeBits,
    unsigned NFractionBits,
    unsigned x )
{
    // Check for 0 and saturate (no overhead for the common case
    //          where x is a constant).
    // might not work for NFractionBits+NWholeBits == 32
    // but that's ok, the algorithm below won't work for NFractionBits>31
    if ( x == 0 ) return (1<<(NFractionBits+NWholeBits))-1;
    // find inverse of y in .31 fixed point... should be enough
    //  precision for all normal uses?
    //  ... this still lets us do a 32/32 division which should be a lot
    //  cheaper than 64/32 ...
    unsigned shift = fixptNBitsPerInt-1;
    unsigned xinverse = ((1U<<shift)/x); // fixpt bits = shift
    return (xinverse >> (shift-NFractionBits));
}

// inverse of unsigned integer x as fixpt number (small case)
//      SMALL means that NFractionBits is <= 30,
//      i.e. this is NOT suitable if you want to use entire wordwidth
//      with no whole bits (0.31 format -- note sign bit makes total of 32).
//      The special case of x==0 is explicitly handled --> saturation.
//      This works well for smaller values of x,
//      losing precision as x gets larger and zeroing out for large x.
static inline ufixpt sfixptInverseOfUIntSmall(
    unsigned NWholeBits,
    unsigned NFractionBits,
    unsigned x )
{
    return ufixptInverseOfUIntSmall(NWholeBits, NFractionBits, x);
}

// inverse of unsigned integer x as fixpt number.
//      This works best for smaller values of x,
//      and doesn't work well at all for large values of x.
//      Because the algorithm doesn't work well for value 1 (and not at 
//      all for 0!) these cases are handled specially to give the
//      saturated value.
static inline ufixptLong ufixptInverseOfUInt(
    unsigned NWholeBits,
    unsigned NFractionBits,
    unsigned x )
{
    // Check for 0 and saturate (no overhead for the common case
    //          where x is a constant).
    if ( x <= 1 ) {
        if ( x == 1 )  {
            /* return fixpt representation of 1 if we can */
            if ( NWholeBits == 0 ) return (1ULL<<(NFractionBits+NWholeBits))-1;
            /* return largets allowable number */
            return (1ULL<<NWholeBits);
        }
        /* here for x == 0.  Return largest allowed number */
        return (1ULL<<(NFractionBits+NWholeBits))-1;
    }
    // find inverse of y in .31 fixed point... should be enough
    //  precision for all normal uses?
    //  ... this still lets us do a 32/32 division which should be a lot
    //  cheaper than 64/32 ...
    unsigned shift = fixptNBitsPerInt-1;
    unsigned long long xinverse = ((1U<<shift)/x); // fixpt bits = shift
    return (xinverse >> (shift-NFractionBits));
}

// inverse of unsigned integer x as fixpt number.
//      This works best for smaller values of x,
//      and doesn't work well at all for large values of x.
//      Because the algorithm doesn't work well for value 1 (and not at 
//      all for 0!) these cases are handled specially to give the
//      saturated value.
static inline sfixptLong sfixptInverseOfUInt(
    unsigned NWholeBits,
    unsigned NFractionBits,
    unsigned x )
{
    return ufixptInverseOfUInt(NWholeBits, NFractionBits, x);
}


// division by unsigned integer  (x/y) for y >= 1.
//      This is efficient only for y a constant,
//      although it is still better than floating point if y is non-constant!
//      This works best for smaller values of y,
//      and doesn't work well at all for large values of y.
static inline ufixptLong ufixptDivByUInt( 
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    unsigned y )
{
    if ( y == 0 ) return (1ULL<<(NFractionBits+NWholeBits))-1;/*saturate*/
    // find inverse of y in .31 fixed point... should be enough
    //  precision for all normal uses?
    //  ... this still lets us do a 32/32 division which should be a lot
    //  cheaper than 64/32 ...
    unsigned shift = fixptNBitsPerInt-1;
    // Although the division is 32/32->32, the multiplication will be 
    // 64*64->64 which is way overkill.
    // The problem is that there is no way to tell the C compiler to do
    // 32*32->64 ; we have to tell the C compiler to do 32*64 at least
    // by making one arg long long; but in practice, gcc raises this to
    // 64x64... sigh... perhaps inline assembler would be a solution.
    unsigned long long yinverse = ((1U<<shift)/y); // fixpt bits = shift
    unsigned long long temp = x*yinverse;  // fixed pt bits = shift+original
    return (temp >> shift);
}

// division by unsigned integer  (x/y) for y >= 1.
//      This is efficient only for y a constant,
//      although it is still better than floating point if y is non-constant!
//      This works best for smaller values of y,
//      and doesn't work well at all for large values of y.
static inline sfixptLong sfixptDivByUInt( 
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    unsigned y )
{
    if ( y == 0 )  {    /* saturate */
        // try to cover it up the best we can? that's the DSP spirit...
        if ( x >= 0 ) return (1ULL<<(NFractionBits+NWholeBits))-1;
        return -((1ULL<<(NFractionBits+NWholeBits))-1);
    }
    // find inverse of y in .31 fixed point... should be enough
    //  precision for all normal uses?
    //  ... this still lets us do a 32/32 division which should be a lot
    //  cheaper than 64/32 ...
    unsigned shift = fixptNBitsPerInt-1;
    // Although the division is 32/32->32, the multiplication will be 
    // 64*64->64 which is way overkill.
    // The problem is that there is no way to tell the C compiler to do
    // 32*32->64 ; we have to tell the C compiler to do 32*64 at least
    // by making one arg long long; but in practice, gcc raises this to
    // 64x64... sigh... perhaps inline assembler would be a solution.
    signed long long yinverse = 
        (unsigned long long)((1U<<shift)/y); // fixpt bits = shift
    signed long long temp = x*yinverse;  // fixed pt bits = shift+original
    // Assume sign bit replicated...
    return (temp >> shift);
}

// division by unsigned integer  (x/y) for y >= 1, with rounding.
//      This is efficient only for y a constant,
//      although it is still better than floating point if y is non-constant!
//      This works best for smaller values of y,
//      and doesn't work well at all for large values of y.
static inline ufixptLong ufixptDivByUIntRound( 
    unsigned NWholeBits,
    unsigned NFractionBits,
    ufixpt x,
    unsigned y )
{
    if ( y == 0 ) return (1ULL<<(NFractionBits+NWholeBits))-1;/*saturate*/
    // find inverse of y in .31 fixed point... should be enough
    //  precision for all normal uses?
    //  ... this still lets us do a 32/32 division which should be a lot
    //  cheaper than 64/32 ...
    unsigned shift = fixptNBitsPerInt-1;
    unsigned long long fraction = (((1U<<shift)+(y>>1))/y);
    unsigned long long temp = x*fraction;  // fixed pt bits = shift+original
    return ufixptRightShiftRoundedLong(NWholeBits, NFractionBits, temp, shift);
}

// division by unsigned integer  (x/y) for y >= 1, with rounding.
//      This is efficient only for y a constant,
//      although it is still better than floating point if y is non-constant!
//      This works best for smaller values of y,
//      and doesn't work well at all for large values of y.
static inline sfixptLong sfixptDivByUIntRound( 
    unsigned NWholeBits,
    unsigned NFractionBits,
    sfixpt x,
    unsigned y )
{
    if ( y == 0 )  {    /* saturate */
        if ( x >= 0 ) return (1ULL<<(NFractionBits+NWholeBits))-1;
        return -((1ULL<<(NFractionBits+NWholeBits))-1);
    }
    // find inverse of y in .31 fixed point... should be enough
    //  precision for all normal uses?
    //  ... this still lets us do a 32/32 division which should be a lot
    //  cheaper than 64/32 ...
    unsigned shift = fixptNBitsPerInt-1;
    signed long long fraction = (unsigned long long)(((1U<<shift)+(y>>1))/y);
    signed long long temp = x*fraction;  // fixed pt bits = shift+original
    return sfixptRightShiftRoundedLong(NWholeBits, NFractionBits, temp, shift);
}

#endif  // fixpt__h

