/*
 * A360651  Triangle T(n, m) = (n - m + 1)*C(2*n + 1, m)*C(2*n - m + 2, n - m
 *          + 1)/(2*n - m + 2).
 * https://oeis.org/A360651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((√(8*n+1)-1)/2), k = n - r*(r+1)/2
 *            and T(r,k) = binomial(r,k) * catalan(r) * (2*r + 1)
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utility prototypes (provided by the build environment) */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* integer square root for 0 <= x < 2^64 */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1ULL << 32);
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* compute row index r */
    idx_t root = isqrt_u64((idx_t)8 * n + 1);
    idx_t r = (root - 1) >> 1;

    /* compute column index k */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* compute triangle entry T(r,k) */
    seq_t bin = binomial(r, k);
    seq_t cat = catalan(r);
    seq_t mult = (seq_t)(2 * (seq_t)r + 1);

    return bin * cat * mult;
}
