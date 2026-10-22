/*
 * A321832  a(n) = Sum_{d|n, n/d==1 (mod 4)} d^8 - Sum_{d|n, n/d==3 (mod 4)}
 *          d^8.
 * https://oeis.org/A321832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, n/d≡1 (mod 4)} d⁸ – Sum_{d|n, n/d≡3 (mod 4)} d⁸
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow8(seq_t x) {
    seq_t x2 = x * x;         /* x² */
    seq_t x4 = x2 * x2;       /* x⁴ */
    return x4 * x4;           /* x⁸ */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;    /* indices < 1 are undefined, but return 0 for safety */

    seq_t add_sum = 0;
    seq_t sub_sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t k = n / d;            /* paired divisor */
        seq_t d_pow8 = pow8((seq_t)d);
        seq_t k_pow8 = pow8((seq_t)k);

        /* contribution from divisor d */
        if ((k & 3U) == 1U) add_sum += d_pow8;
        else if ((k & 3U) == 3U) sub_sum += d_pow8;

        /* avoid double‑counting when d == k */
        if (k != d) {
            /* contribution from paired divisor k */
            if ((d & 3U) == 1U) add_sum += k_pow8;
            else if ((d & 3U) == 3U) sub_sum += k_pow8;
        }
    }

    return add_sum - sub_sum;
}
