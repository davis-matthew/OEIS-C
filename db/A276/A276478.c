/*
 * A276478  Number of points in square lattice in and on the boundary of the
 *          area encompassed by two arcs of radius n and centers at (0,0) and
 *          (n,0).
 * https://oeis.org/A276478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - 3*n - 2*(n-1)*m(n) + 4 * Σ_{k=0..m(n)} floor(sqrt(n²
 *            - k²)), where m(n) = floor(n*sqrt(3)/2) = floor(isqrt(3*n²))/2
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* base case */

    idx_t N = n;
    idx_t m = isqrt(3ULL * N * N) >> 1;  /* m(n) = floor(isqrt(3*N²))/2 */

    seq_t sum = 0;
    for (idx_t k = 0; k <= m; ++k) {
        idx_t rem = N * N - k * k;
        sum += (seq_t)isqrt(rem);
    }

    seq_t res = 4 * sum;
    res -= 3 * (seq_t)N;
    res -= 2 * (seq_t)(N - 1) * (seq_t)m;
    res += 1;
    return res;
}
