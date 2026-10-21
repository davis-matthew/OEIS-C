/*
 * A266594  Total number of ON (black) cells after n iterations of the "Rule
 *          37" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 5 - (-1)^n*(n-3)) / 2 , a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) = 1 (by definition) */
        return (seq_t)1;

    /* Work in the target integer type to avoid intermediate overflow. */
    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;                     /* n^2 */

    /* Compute diff = n - 3 as a signed value to handle negative numbers. */
    long long diff = (long long)n - 3LL;       /* safe for the intended ranges */

    /* term_part = -diff for even n,   +diff for odd n. */
    seq_t term_part = ( (n & 1ULL) ? (seq_t)diff : (seq_t)(-diff) );

    /* numerator = n^2 + 5 + term_part   (mod 2^bits of seq_t) */
    seq_t num = n_sq + (seq_t)5 + term_part;

    return num / (seq_t)2;
}
