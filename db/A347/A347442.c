/*
 * A347442  Number of factorizations of n with integer reverse-alternating
 *          product.
 * https://oeis.org/A347442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts weakly increasing factorizations of n into integers
 *            >1 whose reverse alternating product is an integer
 * Range:     u8 n <= 9215, u16 n <= 9258, u32 n <= 9258, u64 n <= 9258,
 *            u128 n <= 9258, big any n
 * Helpers:   count_adjusted
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_adjusted.h"

/* Public function required by the build system. */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* a(1) = 1 by definition */

    /* A stack big enough for the maximal number of factors (log₂(10⁴) < 14). */
    idx_t stack[64];

    return count_adjusted(n, (idx_t)2, stack, (size_t)0);
}
