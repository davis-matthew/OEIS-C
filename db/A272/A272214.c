/*
 * A272214  Square array read by antidiagonals upwards in which T(n,k) is the
 *          product of the n-th prime and the sum of the divisors of k, n >=
 *          1, k >= 1.
 * https://oeis.org/A272214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 8128, u32 n <= 8128, u64 n <= 8128,
 *            u128 n <= 8128, big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272214: square array read by antidiagonals upwards.
   Term a(n) is the product of the i-th prime and the sum-of-divisors
   of j where (i,j) are obtained from n by the antidiagonal mapping:
   1) Find the smallest d ≥ 2 such that d(d−1)/2 ≥ n.
   2) Let prev = (d−2)(d−1)/2 (number of terms before diagonal d).
   3) offset = n − prev − 1   (0‑based index inside diagonal d).
   4) i = (d−1) − offset,   j = d − i.
   5) a(n) = prime(i) * sigma(j).                               */

#include "../utils/sigma_of.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence starts at n = 1 */

    /* locate diagonal d */
    idx_t d = 2;
    while ((d * (d - 1)) / 2 < n) d++;

    /* number of terms before diagonal d */
    idx_t prev_total = ((d - 2) * (d - 1)) / 2;

    /* offset within diagonal d (0‑based) */
    idx_t offset = n - prev_total - 1;

    /* coordinates (i,j) */
    idx_t i = (d - 1) - offset;
    idx_t j = d - i;

    /* compute prime(i) and sigma(j) */
    seq_t prime_i = (seq_t)get_prime(i);
    seq_t sigma_j = sigma_of(j);

    /* product */
    return prime_i * sigma_j;
}
