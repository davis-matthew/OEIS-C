/*
 * A370702  a(n) = h(n + 1) - h(n) where h(n) = floor(n/sqrt(k) + 1/k) and k =
 *          3.
 * https://oeis.org/A370702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(n + 1) - h(n) where h(n) = floor(n / sqrt(k) + 1/k) and
 *            k = 3
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define K 3

/* Return floor(n / sqrt(k)) using only integer arithmetic. */
static inline idx_t floor_sqrt_over_k(idx_t n) {
    __uint128_t n2 = (__uint128_t)n * n;         /* n^2 */
    __uint128_t limit = n2 / K;                  /* floor(n^2 / k) */
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= limit) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high; /* highest mid such that mid^2 * k <= n^2 */
}

/* Compute h(n) = floor(n / sqrt(k) + 1/k) */
static inline seq_t h_value(idx_t n) {
    idx_t d = floor_sqrt_over_k(n);             /* floor(n / sqrt(k)) */

    /* Check whether the fractional part of n / sqrt(k) is ≥ (k-1)/k = 2/3 */
    __uint128_t left  = (__uint128_t)K * ( (__uint128_t)n * n );   /* K * n^2 */
    __uint128_t thresh = (__uint128_t)K * d + (K - 1);           /* K*d + K - 1 */
    __uint128_t right = thresh * thresh;                         /* (K*d + K - 1)^2 */

    seq_t t = (left >= right) ? 1 : 0;   /* 1 if fractional part ≥ 2/3, else 0 */
    return (seq_t)(d + t);
}

/* Main sequence function */
seq_t a(idx_t n) {
    return h_value(n + 1) - h_value(n);
}
