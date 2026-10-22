/*
 * A368479  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} 2^j * j^k.
 * https://oeis.org/A368479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i, j) where (i, j) satisfy n = i + (i + j)(i + j +
 *            1)/2, and T(i, j) = Σ_{p=0}^{i} 2^p * p^j. The array is read
 *            along antidiagonals downwards
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   triangle_offset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangle_offset.h"

seq_t a(idx_t n) {
    /* Find d such that C(d-1) <= n < C(d)  (minimal d with C(d) > n) */
    idx_t low = 0;
    idx_t high = n;  /* d never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangle_offset(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t d = low; /* minimal d with triangle_offset(d) > n */

    /* Previous cumulative count: C(d-1) = d(d+1)/2 */
    idx_t prev = (idx_t)(((__uint128_t)d * (d + 1)) / 2);
    idx_t offset = n - prev;      /* position inside antidiagonal d */
    idx_t i = offset;             /* row index */
    idx_t j = d - offset;         /* column index */

    /* Compute T(i, j) = Σ_{p=0}^{i} 2^p * p^j */
    seq_t sum = 0;
    seq_t pow2 = 1;               /* 2^p */
    for (idx_t p = 0; p <= i; ++p) {
        seq_t pow_p_j = 1;
        for (idx_t k = 0; k < j; ++k)
            pow_p_j *= (seq_t)p;
        sum += pow2 * pow_p_j;
        pow2 *= 2;
    }
    return sum;
}
