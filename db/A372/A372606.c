/*
 * A372606  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} phi(k*j).
 * https://oeis.org/A372606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{t=1..i} phi(j*t), where (i,j) are the row/column of
 *            n in the antidiagonal (downward) ordering of the array T(n,k) =
 *            Sum_{r=1..n} phi(k*r)
 * Range:     u8 n <= 146, u16 n <= 6398, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that the first s*(s+1)/2 terms cover n. */
    idx_t s = 1;
    while (((unsigned __int128)s * (s + 1)) / 2 < n) ++s;

    /* Elements before this diagonal: (s-1)*s/2 */
    idx_t prev = ((unsigned __int128)(s - 1) * s) / 2;

    /* Zero‑based offset within diagonal s */
    idx_t r = n - prev - 1;       /* 0 <= r < s */

    /* Coordinates in the array: row i = r+1, column j = s - r */
    idx_t i = r + 1;
    idx_t j = s - r;

    /* Compute T(i,j) = Σ_{t=1..i} φ(j*t) */
    unsigned __int128 sum = 0;
    for (idx_t t = 1; t <= i; ++t) {
        idx_t prod = j * t;               /* j*t fits in idx_t for given bounds */
        sum += (unsigned __int128)phi((idx_t)prod);
    }

    return (seq_t)sum;
}
