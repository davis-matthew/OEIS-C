/*
 * A331801  Integers that are sum of two nonsquarefree numbers.
 * https://oeis.org/A331801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A331801: integers that are a sum of two nonsquarefree numbers.
   Formula: for n >= 10 we have a(n) = n + 14,
   while the first nine terms are
   8, 12, 13, 16, 17, 18, 20, 21, 22. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Table of the first nine terms. */
    static const seq_t first[9] = {8,12,13,16,17,18,20,21,22};

    if (n <= 9)
        return first[n - 1];

    /* For n >= 10: a(n) = n + 14. */
    return (seq_t)n + (seq_t)14;
}
