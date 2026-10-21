/*
 * A281725  Triangular array T(n,k) is the sum of elements in an n X k matrix
 *          that will be assigned the same value whether the integers from 1
 *          to n*k are assigned to elements in row-major order or column-major
 *          order.
 * https://oeis.org/A281725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum of values (i-1)*k + j over all i,j such that (i-1)*k
 *            + j = (j-1)*r + i, where r and k are extracted from n via r = n
 *            - k*(k-1)/2, and k = ceil((sqrt(8*n+1)-1)/2). (r ≤ k) The index
 *            n is 1‑based; a(0) is undefined and returns 0
 * Range:     u8 n <= 35, u16 n <= 1325, u32 n <= 9712, u64 n <= 9712,
 *            u128 n <= 9712, big any n
 * Helpers:   get_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/get_k.h"

/*--------------------------------------------------------------------
 *  Helper: integer square root of a 128‑bit value (returns floor(sqrt(x)))
 *--------------------------------------------------------------------*/
static inline idx_t isqrt_u128(__int128 x) {
    idx_t low = 0, high = (idx_t)1 << 32;   /* sqrt(2^64) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if ((__int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/*--------------------------------------------------------------------
 *  Compute T(r,k) – the sum of common elements in an r × k matrix
 *--------------------------------------------------------------------*/
static inline seq_t compute_T(idx_t r, idx_t k)
{
    /* special cases where whole matrix matches */
    if (r == 1) return (seq_t)k * ((seq_t)k + 1) / 2;
    if (k == 1) return (seq_t)r * ((seq_t)r + 1) / 2;

    seq_t sum = 0;
    int64_t denom = (int64_t)(r - 1);                  /* r > 1 */
    for (idx_t i = 1; i <= r; ++i) {
        __int128 num = (__int128)i * (k - 1) - (__int128)(k - r);
        if (num % denom != 0) continue;
        __int128 j = num / denom;
        if (j < 1 || j > k) continue;

        seq_t val = (seq_t)((__int128)(i - 1) * k + (idx_t)j);
        sum += val;
    }
    return sum;
}

/*--------------------------------------------------------------------
 *  Required public function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, 0 is undefined */
    idx_t k = get_k(n);
    /* previous number of entries before column k */
    __int128 prev128 = (__int128)k * (k - 1) / 2;
    idx_t prev = (idx_t)prev128;
    idx_t r = n - prev;                  /* 1 ≤ r ≤ k */
    return compute_T(r, k);
}
