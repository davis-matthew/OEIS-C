/*
 * A293234  a(n) is the number of proper divisors of n that are square.
 * https://oeis.org/A293234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d : d | n, d < n, d is a perfect square }
 * Range:     u8 n <= 9044, u16 n <= 9044, u32 n <= 9044, u64 n <= 9044,
 *            u128 n <= 9044, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) using 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t y = x | (1ULL << i);
        __uint128_t y2 = (__uint128_t)y * y;
        if (y2 <= n) x = y;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t temp = n;
    seq_t prod = 1;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            prod *= (seq_t)(exp / 2 + 1);
        }
    }

    /* Remaining prime factor (if any) contributes a factor of 1, so prod stays unchanged. */

    /* Exclude n itself if it is a perfect square. */
    idx_t sq = isqrt_u64(n);
    if ((seq_t)sq * (seq_t)sq == n) {
        prod -= 1;
    }

    return prod;
}
