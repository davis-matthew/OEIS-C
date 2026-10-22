/*
 * A386250  Total number of ones in runs of 1's of length >= 4 over all binary
 *          strings of length n.
 * https://oeis.org/A386250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for OEIS A386250:
   a(0)=0, a(1)=0, a(2)=0, a(3)=0,
   a(4)=4,
   for n≥5  a(n) = (5*n - 12) * 2^(n-5)  (integer value)
   (equivalently, a(n) = 4*a(n-1) - 4*a(n-2) for n≥6). */

#include "../utils/pow2.h"  /* provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n) {
    if (n < 4)              /* n=0,1,2,3 */
        return 0;
    if (n == 4)              /* special case because 2^(n-5)=2^(-1) */
        return 4;
    /* n ≥ 5 */
    seq_t factor = (seq_t)(5 * n - 12);   /* 5*n-12 fits in idx_t */
    seq_t power  = pow2(n - 5);           /* 2^(n-5) */
    return factor * power;
}
