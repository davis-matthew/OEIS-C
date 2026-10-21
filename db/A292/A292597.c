/*
 * A292597  a(1) = 1; for n > 1, a(n) = c(n) + 2*a(floor(n/2)), where c(n) is
 *          the characteristic function of odd composites, A071904.
 * https://oeis.org/A292597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = c(n) + 2*a(floor(n/2)), where c(n) =
 *            1 if n is an odd composite, otherwise 0
 * Range:     u8 n <= 255, u16 n <= 7653, u32 n <= 7653, u64 n <= 7653,
 *            u128 n <= 7653, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

static bool is_odd_composite(idx_t n)
{
    if (n < 3) return false;          /* 1, 2 are not composites */
    if ((n & 1) == 0) return false;   /* even numbers are excluded */
    return !is_prime_small((seq_t)n);  /* composite if not prime */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;       /* base case */

    /* Recursive half term */
    seq_t half = a(n / 2);

    /* Multiply by 2 and add the characteristic value */
    seq_t res = half + half;           /* 2 * half */

    if (is_odd_composite(n))
        res += (seq_t)1;

    return res;
}
