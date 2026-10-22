/*
 * A307136  a(n) = ceiling(2*sqrt(A000037(n))), n >= 1.
 * https://oeis.org/A307136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(2*sqrt(A000037(n)))
 * Range:     u8 n <= 587, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values.
   Returns floor(sqrt(n)).  Uses binary search with 128‑bit
   multiplication to avoid overflow. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* a(n) for n >= 1.  Uses the definition
   A000037(n) = the n‑th non‑square positive integer. */
seq_t a(idx_t n)
{
    /* Find k such that k(k-1) <= n < k^2 + k + 1.
       k = floor((1 + floor(sqrt(1+4n)))/2). */
    idx_t t = 1 + 4 * n;
    idx_t s_tmp = isqrt_u64(t);      /* sqrt(1+4n) floor */
    idx_t k = (1 + s_tmp) / 2;       /* integer division */

    /* Candidate for the n‑th non‑square */
    idx_t m = n + k;
    idx_t sqrt_m = isqrt_u64(m);
    if (sqrt_m * sqrt_m == m)      /* m is a perfect square, skip it */
        m--;

    seq_t D = (seq_t)m;             /* A000037(n), non‑square integer */

    /* Compute ceil(2*sqrt(D)) without floating point. */
    idx_t s = isqrt_u64((idx_t)D);  /* floor(sqrt(D)) */
    seq_t s_seq = (seq_t)s;
    seq_t threshold = s_seq * (seq_t)(s + 1);  /* s^2 + s */

    seq_t ans;
    if (D <= threshold)
        ans = (seq_t)(2 * s + 1);
    else
        ans = (seq_t)(2 * s + 2);

    return ans;
}
