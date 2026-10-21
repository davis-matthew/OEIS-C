/*
 * A254876  Triangular table T(n,k) = n! / Product_{m=(n-floor((2n)/(3^k))) ..
 *          (n-floor((n)/(3^k)))} m, read by rows T(1,1), T(2,1), T(2,2),
 *          T(3,1), T(3,2), T(3,3), ...
 * https://oeis.org/A254876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(rows, cols) where rows, cols are obtained by reading
 *            the triangular table T(n,k) = n! / prod_{m = n -
 *            floor(2n/3^k)}^{n - floor(n/3^k)} m in row‑major order: T(1,1),
 *            T(2,1), T(2,2), T(3,1), T(3,2), T(3,3),
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 94, u64 n <= 234,
 *            u128 n <= 633, big any n
 * Helpers:   pow3_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow3_limit.h"

seq_t a(idx_t n)
{
    /* determine rows and cols from triangular indexing */
    idx_t r = 1;
    while ((__int128)r * (r + 1) / 2 < n)
        ++r;
    idx_t prev = (idx_t)((__int128)(r - 1) * r / 2);
    idx_t k    = n - prev;   /* 1 ≤ k ≤ r */

    /* compute T(r,k) */
    /* first compute floor(2*r/3^k) and floor(r/3^k) using clamped power */
    seq_t pow3 = pow3_limit(k, (idx_t)(2 * r));
    seq_t floor2n_seq, floorn_seq;
    if (pow3 > (seq_t)(2 * r))
        floor2n_seq = 0;
    else
        floor2n_seq = ((seq_t)(2 * r)) / pow3;

    if (pow3 > (seq_t)r)
        floorn_seq = 0;
    else
        floorn_seq = ((seq_t)r) / pow3;

    /* convert to idx_t (safe for our ranges) */
    idx_t floor2n = (idx_t)floor2n_seq;
    idx_t floorn  = (idx_t)floorn_seq;

    idx_t L = r - floor2n;
    idx_t R = r - floorn;

    /* compute the product of all numbers outside the segment [L,R] */
    seq_t result = 1;
    for (idx_t m = 1; m <= r; ++m)
    {
        if (m < L || m > R)
            result *= (seq_t)m;
    }
    return result;
}
