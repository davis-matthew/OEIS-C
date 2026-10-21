/*
 * A286476  Compound filter: a(n) = 6*A032742(n) + (n mod 6), a(1) = 1.
 * https://oeis.org/A286476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * (n / spf(n)) + (n mod 6), with spf(n) = smallest
 *            prime factor of n; a(1) = 1
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n < 2)            /* a(1) = 1 */
        return (seq_t)1;

    /* Find smallest prime factor of n */
    idx_t spf = n;                    /* default: n is prime */
    idx_t limit = isqrt_u64(n);
    for (idx_t i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }

    /* Compute the sequence value */
    seq_t part = (seq_t)(n / spf);         /* n / spf fits in idx_t */
    return (seq_t)6 * part + (seq_t)(n % 6);
}
