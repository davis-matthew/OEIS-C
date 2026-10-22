/*
 * A322071  Triangle read by rows: T(n, k) is the largest integer m such that
 *          m*k^k <= 2*n^k.
 * https://oeis.org/A322071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(2 * r^k / k^k), with (r,k) determined from n by
 *            reading the triangle T(r,k) = floor(2*r^k / k^k) row‑wise
 *            (offset 1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 63, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

 /* Compute the entry T(r,k) = floor(2*r^k/k^k). */
static seq_t triangle_value(idx_t r, idx_t k)
{
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;

    /* r^k and k^k (big integer arithmetic). */
    seq_t pow_r_k = big_pow(sr, sk);
    seq_t pow_k_k = big_pow(sk, sk);

    /* Numerator 2 * r^k. */
    seq_t numerator = ((seq_t)2) * pow_r_k;

    /* Floor division. */
    return numerator / pow_k_k;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used; offset is 1 */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t tri = 1;                         /* triangular number r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;                          /* next triangular number */
    }

    /* Determine the column k within that row. */
    idx_t prev_tri = tri - r;              /* (r-1)*r/2 */
    idx_t k = n - prev_tri;

    return triangle_value(r, k);
}
