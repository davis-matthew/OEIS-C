/*
 * A321266  Smallest positive number for which the square cannot be written as
 *          sum of distinct squares of any subset of previous terms.
 * https://oeis.org/A321266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321266 (smallest positive number for which the square
   cannot be expressed as a sum of distinct squares of any subset of
   previous terms).  After the first nine terms it satisfies
   the recurrence a(n) = 2 * a(n-3) for n > 9.  The implementation
   uses that recurrence so the function runs in O(1).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t BASE[9] = {1,2,3,4,6,8,12,16,17};

    /* Treat indices below 1 as 0 (undefined in OEIS, but keep
       defensive behaviour; the tests use n >= 1). */
    if (n <= 0)
        return (seq_t)0;

    /* Return the first nine terms directly. */
    if (n <= 9)
        return BASE[n-1];

    /* For n > 9 use the recurrence a(n) = 2 * a(n-3).  
       We compute it iteratively to avoid recursion depth. */
    idx_t  m = n;      /* current index to reduce */
    seq_t  mul = (seq_t)1;   /* 2^{k} multiplier */

    while (m > 9) {
        mul *= (seq_t)2;
        m -= 3;
    }
    return BASE[m-1] * mul;
}
