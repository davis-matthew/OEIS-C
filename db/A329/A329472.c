/*
 * A329472  Partial sums of numbers that are not squarefree.
 * https://oeis.org/A329472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 225, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: partial sums of numbers that are not squarefree.
   The function scans integers, sums those divisible by a square, and
   returns the sum of the first n such numbers. */

#include <stdbool.h>

static bool is_squarefree(idx_t x)
{
    /* Return true if x is squarefree, i.e. not divisible by any square > 1. */
    for (idx_t p = 2; p <= x / p; ++p) {
        if (x % (p * p) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    idx_t count = 0;
    idx_t num = 1;            /* start checking from 1 */

    while (count < n) {
        ++num;
        if (!is_squarefree(num)) {
            ++count;
            sum += (seq_t)num;
        }
    }

    return sum;
}
