/*
 * A373340  Number of permutations of symmetric group S_n with an odd number
 *          of cycles of length 2 or more.
 * https://oeis.org/A373340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / 2 + (n-2) * 2^(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the only permutation has zero non‑fixed
       point cycles, so the result is 0. */
    if (n < 2)
        return 0;

    /* Compute n! and divide by 2 (integer division). */
    seq_t fact = factorial(n);
    seq_t part1 = fact / 2;

    /* Compute 2^(n-2) and multiply by (n-2). */
    seq_t pow = pow2(n - 2);
    seq_t part2 = (seq_t)(n - 2) * pow;

    /* Sum the two parts to obtain a(n). */
    return part1 + part2;
}
