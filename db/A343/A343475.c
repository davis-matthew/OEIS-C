/*
 * A343475  a(n) is the number of preference profiles for n men and n women,
 *          where men prefer distinct women as their first choice.
 * https://oeis.org/A343475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^(n+1) * ((n-1)!)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 (OEIS offset 1).
       For n = 0 the function returns 0, but this value is unused. */
    if (n == 0) return 0;

    /* Compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* Compute (n-1)! as fact_n / n.  This is valid for n ≥ 1. */
    seq_t fact_n1 = fact_n / (seq_t)n;

    /* Raise the factorials to the required powers. */
    seq_t part1 = big_pow(fact_n, (seq_t)(n + 1));
    seq_t part2 = big_pow(fact_n1, (seq_t)n);

    /* Multiply the two parts to get a(n). */
    part1 *= part2;
    return part1;
}
