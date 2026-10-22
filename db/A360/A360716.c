/*
 * A360716  Number of unordered pairs of self-avoiding paths whose sets of
 *          nodes are disjoint subsets of a set of n points on a circle;
 *          one-node paths are not allowed.
 * https://oeis.org/A360716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/32 * (5^(n-2) - 2*3^(n-2) + 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses pow_int and gcd utilities for arbitrary‑precision support. */
#include "../utils/pow_int.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Offset 1 – for n = 1,2,3 the value is 0. */
    if (n < 4) return 0;

    /* Convert index to seq_t early. */
    seq_t N = (seq_t)n;

    /* Basic factors. */
    seq_t term = N * (N - 1);          /* n*(n-1)         */
    seq_t pow5 = pow_int((seq_t)5, n - 2);   /* 5^(n-2)         */
    seq_t pow3 = pow_int((seq_t)3, n - 2);   /* 3^(n-2)         */

    /* diff = 5^(n-2) - 2*3^(n-2) + 1 */
    seq_t diff = pow5 + 1;
    diff -= pow3 * 2;

    /* Cancel the factor 32 before the final multiplication
       to avoid overflow on 64‑bit builds. */
    seq_t g1 = gcd(term, (seq_t)32);
    term /= g1;
    seq_t denom1 = (seq_t)32 / g1;          /* remaining part of 32 */

    seq_t g2 = gcd(diff, denom1);
    diff /= g2;
    seq_t denom2 = denom1 / g2;             /* should be 1 for all n */

    /* In the unlikely event denom2 > 1, fall back to a 128‑bit
       intermediate; this code path will never be taken for terms
       that fit into seq_t=uint64_t (n <= 27). */
    if (denom2 != 1)
        return (term * diff) / denom2;

    return term * diff;
}
