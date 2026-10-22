/*
 * A347502  Number of dominating sets in the n-cycle complement graph.
 * https://oeis.org/A347502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - 2*n - 1 for n != 4, with a(4) = 9
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 4)
        return (seq_t)9;

    /* For small integer types use bit‑shift, otherwise use big_pow() */
    if (sizeof(seq_t) <= 8) {
        unsigned int bits = 8 * sizeof(seq_t);
        /* Ensure shift stays within the signed range */
        if (n < bits - 1) {
            seq_t pow = (seq_t)1 << n;               /* 2^n */
            return pow - (seq_t)2 * (seq_t)n - (seq_t)1;
        }
    }

    /* Fallback: arbitrary‑precision (or overflow branch) */
    seq_t pow = big_pow((seq_t)2, (seq_t)n);        /* 2^n */
    return pow - (seq_t)2 * (seq_t)n - (seq_t)1;
}
