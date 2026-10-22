/*
 * A361731  Array read by descending antidiagonals. A(n, k) = hypergeom([-k,
 *          -3], [1], n).
 * https://oeis.org/A361731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(r,c) where * A(n,k) = 1 + (((k*n - 3*n + 9)*n*k + (2*n
 *            - 9)*n + 18)*n*k)/6 * The array is read by descending
 *            antidiagonals: * r = offset, c = s - offset, s is the largest
 *            integer with s*(s+1)/2 ≤ n
 * Range:     u8 n <= 16, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   compute_A
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>     /* for idx_t, seq_t definitions used by the build */
#include "../utils/compute_A.h"

static idx_t triangular_root(idx_t n)
{
    /* Find the largest m with m*(m+1)/2 ≤ n. */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    low = high >> 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t val = (__int128)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t s = triangular_root(n);
    idx_t tri = (idx_t)((__int128)s * (s + 1) / 2);
    idx_t offset = n - tri;
    idx_t r = offset;
    idx_t c = s - offset;
    return compute_A(r, c);
}
