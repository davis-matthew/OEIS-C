/*
 * A400876  Array read by ascending antidiagonals: A(n,k) = denominator((n^2 -
 *          2*n*k - k^2)/(n^2 + k^2)), with k > 0.
 * https://oeis.org/A400876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl, new
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Denominator of (n^2-2*n*k-k^2)/(n^2+k^2) read by ascending
 * antidiagonals (n,k≥1).  a(n) returns that denominator.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit x */
static idx_t floor_isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* hi > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Integer square root: ceil(sqrt(x)) for unsigned 64‑bit x */
static idx_t ceil_isqrt_u64(idx_t x) {
    idx_t r = floor_isqrt_u64(x);
    if ((unsigned __int128)r * r < x)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Find diagonal number s such that (s-1)*s/2 >= n */
    idx_t sqrtD = ceil_isqrt_u64((idx_t)(((__int128)8 * n) + 1));
    idx_t temp = 1 + sqrtD;
    idx_t s = temp >> 1;
    if (temp & 1)
        ++s;                      /* ceil((1+sqrt)/2) */

    /* Number of terms before this diagonal */
    idx_t prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;

    /* Position within the diagonal (1‑based) */
    idx_t offset = n - prev;          /* 1 ≤ offset ≤ s-1 */

    /* Indices (row, col) = (n, k) */
    idx_t row = offset;              /* n */
    idx_t col = s - offset;          /* k */

    /* Compute numerator and denominator of the fraction */
    int64_t n_i = (int64_t)row;
    int64_t k_i = (int64_t)col;
    __int128 num_signed = (__int128)n_i * n_i
                         - (__int128)2 * n_i * k_i
                         - (__int128)k_i * k_i;
    __int128 den_signed = (__int128)n_i * n_i + (__int128)k_i * k_i;

    seq_t abs_num = num_signed >= 0 ? (seq_t)num_signed
                                    : (seq_t)(-num_signed);
    seq_t den = (seq_t)den_signed;

    seq_t g = gcd(abs_num, den);
    return den / g;
}
