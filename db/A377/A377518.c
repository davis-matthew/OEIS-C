/*
 * A377518  The largest divisor of n that is a term in A207481.
 * https://oeis.org/A377518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p^e) = p^min(p, e)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root, floor, for 64‑bit values */
static inline idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (n >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;               /* a(1) = 1 */

    seq_t result = 1;
    idx_t temp = n;
    idx_t limit = isqrt_u64(temp);

    for (idx_t d = 2; d <= limit; ++d) {
        if (temp % d == 0) {
            idx_t exp = 0;
            while (temp % d == 0) {
                temp /= d;
                ++exp;
            }
            idx_t use = (exp < d) ? exp : d;   /* min(p, e) */
            seq_t power = 1;
            for (idx_t i = 0; i < use; ++i)
                power *= (seq_t)d;
            result *= power;
            limit = isqrt_u64(temp);          /* update limit after division */
        }
    }

    if (temp > 1) {                   /* remaining prime factor */
        seq_t power = temp;
        result *= power;
    }

    return result;
}
