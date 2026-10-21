/*
 * A257175  The smallest m such that the m-th triangular number is greater
 *          than or equal to half the n-th triangular number.
 * https://oeis.org/A257175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((sqrt(2n^2 + 2n + 1) - 1)/2)
 * Range:     u8 n <= 5, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Computes the n-th term of OEIS A257175.
 * Uses integer arithmetic only; safe for 8‑, 16‑, 32‑, 64‑ or 128‑bit seq_t,
 * as well as arbitrary‑precision seq_t. */
seq_t a(idx_t n)
{
    /* Convert index to the potentially larger seq_t type */
    seq_t nn = (seq_t)n;

    /* s = 2*n^2 + 2*n + 1 */
    seq_t s = nn * nn;          /* n^2 */
    s = s * 2;                  /* 2*n^2 */
    seq_t tmp = nn * 2;         /* 2*n */
    s += tmp;                   /* 2*n^2 + 2*n */
    s += 1;                     /* 2*n^2 + 2*n + 1 */

    /* r = floor(sqrt(s)) */
    seq_t r = floor_sqrt(s);

    /* If s is a perfect square, use (r-1)/2; otherwise use (r+1)/2. */
    if (r * r == s)
        return (r - 1) / 2;
    else
        return (r + 1) / 2;
}
