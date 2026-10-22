/*
 * A306810  Inverse binomial transform of the continued fraction expansion of
 *          e.
 * https://oeis.org/A306810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A306810: Inverse binomial transform of the continued fraction expansion of e.
   The sequence satisfies the linear recurrence
       a(n) = -7*a(n-1) -21*a(n-2) -33*a(n-3) -27*a(n-4) -9*a(n-5)   for n >= 7
   with initial values
       a(0)=2, a(1)=-1, a(2)=2, a(3)=-4, a(4)=7, a(5)=-8, a(6)=-2
   This implementation uses memoization and is O(n) per call after the first.
*/

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *seq = NULL;
    static idx_t size = 0;          /* largest index currently stored */

    /* initialisation: store known values up to a(6) */
    if (size == 0) {
        size = 6;                    /* highest pre‑computed index */
        seq = (seq_t *)malloc(sizeof(seq_t) * (size + 1));
        seq[0] = (seq_t) 2;
        seq[1] = (seq_t)-1;
        seq[2] = (seq_t) 2;
        seq[3] = (seq_t)-4;
        seq[4] = (seq_t) 7;
        seq[5] = (seq_t)-8;
        seq[6] = (seq_t)-2;
    }

    if (n <= size)
        return seq[n];

    /* extend the cache up to index n */
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * (n + 1));
    for (idx_t i = size + 1; i <= n; ++i) {
        /* a(i) = -7*a(i-1) -21*a(i-2) -33*a(i-3) -27*a(i-4) -9*a(i-5) */
        seq[i] = (seq_t)(-7) * seq[i - 1]
              + (seq_t)(-21) * seq[i - 2]
              + (seq_t)(-33) * seq[i - 3]
              + (seq_t)(-27) * seq[i - 4]
              + (seq_t)(-9)  * seq[i - 5];
    }
    size = n;
    return seq[n];
}
