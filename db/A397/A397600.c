/*
 * A397600  Number of increasing 3-term arithmetic progressions in an n^d
 *          hypercube, array A(n,d) read by antidiagonals.
 * https://oeis.org/A397600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ((floor(N/2)^2 + ceil(N/2)^2)^D - N^D) / 2 where (N,D) is
 *            the pair (n,d) corresponding to the nth entry of the array read
 *            by antidiagonals
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 82, u64 n <= 196,
 *            u128 n <= 504, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the pair (N,D) corresponding to the nth entry.
       Enumerate all (N,D) with N,D ≥1 sorted by s = N + D,
       and within each s increasing N.  The cumulative number of
       pairs with sum ≤ s is (s-1)*s/2.  Find the minimal s with
       that ≥ n. */
    idx_t s = 2;                 /* current sum N + D */
    idx_t cum = 1;               /* cumulative count up to current sum (s) */

    while (n > cum) {
        ++s;
        cum += s - 1;            /* add the new row of (s-1) pairs */
    }

    /* cumulative up to previous sum is cum - (s-1) */
    idx_t prev_cum = cum - (s - 1);
    idx_t offset = n - prev_cum - 1;  /* zero‑based offset inside this sum */

    idx_t N = offset + 1;
    idx_t D = s - N;

    /* Compute floor(N/2) and ceil(N/2) */
    idx_t x = N >> 1;          /* floor division by 2 */
    idx_t y = N - x;           /* ceil division by 2 */

    seq_t xx = (seq_t)x;
    seq_t yy = (seq_t)y;
    seq_t sum_sq = xx * xx + yy * yy;

    /* Raise to the power D using the provided big integer power function */
    seq_t pow1 = big_pow(sum_sq, (seq_t)D);      /* (x^2 + y^2)^D */
    seq_t pow2 = big_pow((seq_t)N, (seq_t)D);    /*  N^D        */

    seq_t diff = pow1 - pow2;
    seq_t ans = diff / 2;
    return ans;
}
