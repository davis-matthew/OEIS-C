/*
 * A308281  The third power of the unsigned Lah triangular matrix A105278.
 * https://oeis.org/A308281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = flatten of the triangle with entries T(m,k) = 3^(m-k) *
 *            binomial(m-1,k-1) * m! / k! triangular coefficients are taken
 *            row-wise starting at n=1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"  /* seq_t binomial(idx_t a, idx_t b); */

static inline idx_t floor_sqrt_u64(idx_t x) {
    idx_t low = 0, high = (idx_t)1 << 32;   /* sqrt(UINT64_MAX) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t disc = 8 * n + 1;
    idx_t sqrt_disc = floor_sqrt_u64(disc);
    if ((__int128)sqrt_disc * sqrt_disc < disc) sqrt_disc++;   /* ceil sqrt */
    idx_t r = (sqrt_disc - 1) >> 1;
    if (r * (r + 1) / 2 < n) r++;             /* adjust if needed */

    /* Column j in that row (1-indexed) */
    idx_t start_of_row = (r - 1) * r / 2;
    idx_t j = n - start_of_row;                /* 1 <= j <= r */

    /* Compute T(r,j) */
    idx_t diff = r - j;                       /* exponent of 3 */
    seq_t res = 1;

    /* 3^(diff) */
    for (idx_t i = 0; i < diff; ++i)
        res *= (seq_t)3;

    /* Multiply by binomial(r-1, j-1) */
    res *= binomial(r - 1, j - 1);

    /* Multiply by (j+1)*(j+2)*...*r  (equals r! / j!) */
    for (idx_t k = j + 1; k <= r; ++k)
        res *= (seq_t)k;

    return res;
}
