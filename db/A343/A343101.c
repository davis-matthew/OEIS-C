/*
 * A343101  Pairs of integers (k, m) ordered by m with 1 < k < m such that k
 *          has the same prime divisors as m, and, k+1 has the same prime
 *          divisors as m+1.
 * https://oeis.org/A343101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, hard, more
 * Formula:   a(n) = the k or m value of the Benelux pair with the *
 *            (n‑1)/2–th smallest m. All pairs except the unique exception *
 *            are given by (k,m) = (2^q‑2 , 2^q·(2^q‑2)) for q ≥ 2. * The lone
 *            exceptional pair (75,1215) lies between the pairs * for q = 5
 *            and q = 6 in the ordering by m
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Index of the pair in the sorted list by m. */
    idx_t pair_idx = (n - 1) / 2;
    /* 1 for odd n (k) and 0 for even n (m). */
    idx_t odd = n & 1U;

    /* Handle the unique exceptional pair (75, 1215). */
    if (pair_idx == 4)
        return odd ? (seq_t)75 : (seq_t)1215;

    /* Determine q for the infinite family. */
    idx_t q;
    if (pair_idx <= 3)
        q = pair_idx + 2;   /* pairs for q = 2,3,4,5 */
    else
        q = pair_idx + 1;   /* pairs for q ≥ 6  (shifted by one after the exception) */

    /* Compute k = 2^q - 2. */
    seq_t k = pow2(q);
    k -= (seq_t)2;

    /* Compute m = 2^q * (2^q - 2) = 2^(2q) - 2^(q+1). */
    seq_t m  = pow2(2 * q);
    seq_t tmp = pow2(q + 1);
    m -= tmp;

    return odd ? k : m;
}
