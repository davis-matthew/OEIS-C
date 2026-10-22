/*
 * A309773  n directly precedes a(n) in Sharkovskii ordering.
 * https://oeis.org/A309773
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A309773:  a((2*m+1)*2^k) = (2*m+3)*2^k  for m>0, 
   a(2^(k+1)) = 2^k  (i.e. a(2)=1, a(4)=2, a(8)=4, ...).  */
/* The sequence is defined for n >= 2, with offset 2. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be at least 2; behaviour for n<2 is undefined.*/
    idx_t temp = n;
    idx_t v = 0;          /* power of two exponent (number of trailing zeros) */
    while ((temp & 1) == 0) {
        ++v;
        temp >>= 1;
    }
    /* now temp is the odd part of n, i.e. temp = 2*m+1  (m>=0) */
    seq_t odd = (seq_t)temp;
    if (odd == 1) {
        /* n = 2^(k+1) --> a(n) = n/2 */
        return (seq_t)n >> 1;
    } else {
        /* n = (2*m+1)*2^k --> a(n) = (2*m+3)*2^k = (odd+2) * 2^k */
        return ((seq_t)(odd + 2)) << v;
    }
}
