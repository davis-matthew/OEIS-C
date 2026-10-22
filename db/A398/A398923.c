/*
 * A398923  Number of distinct acute lattice trapezia of area at most n/2 with
 *          horizontal base.
 * https://oeis.org/A398923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = |{ (a,b,c,d) in N^4 : a*(b+2c+d) <= n }|
 *              (all variables >= 1)
 *
 * The sequence counts solutions to the inequality
 * a*(b + 2c + d) <= n with a,b,c,d >= 1.
 * This is equivalent to counting, for each a, the number of triples
 * (b,c,d) such that b + 2c + d <= floor(n/a).
 * The inner counting is performed in O(1) with a short loop over y =
 * c-1, leading to an overall O(n log n) algorithm.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)        /* for n = 1..3 there are no solutions */
        return 0;

    seq_t total = 0;

    for (idx_t a = 1; a <= n; ++a)
    {
        idx_t m = n / a;          /* maximum allowed value of b+2c+d   */
        if (m < 4)                /* need at least 1+2*1+1 = 4 */
            continue;

        idx_t M = m - 4;          /* transform to non‑negative variables */
        seq_t count = 0;

        /* For each possible y = c-1 (>=0) up to floor(M/2)                       */
        /* y runs from 0 to t1.  For a fixed y, let K = M - 2*y.  Then             */
        /* the number of non‑negative (x,b-1,d-1) with x+z <= K is (K+1)(K+2)/2    */
        idx_t t1 = M >> 1;        /* floor(M/2) */

        for (idx_t y = 0; y <= t1; ++y)
        {
            idx_t K = M - (y << 1);                /* M - 2*y */
            seq_t term = ((seq_t)(K + 2) * (seq_t)(K + 1)) >> 1; /* (K+2)(K+1)/2 */
            count += term;
        }

        total += count;
    }

    return total;
}
