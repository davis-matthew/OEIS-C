/*
 * A272671  Numbers k such that the decimal number 1k is a square.
 * https://oeis.org/A272671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 128, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k for which the decimal concatenation of '1' and k is a perfect square.
   This number is found by enumerating squares s² such that s²−10ᵈ has exactly d digits, where d is the number
   of decimal digits of k.  The sequence is generated on demand. */

#include <stdint.h>

/* integer square‑root for 64‑bit unsigned values – floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = (idx_t)1 << 62;           /* highest power of four <= max  */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n  -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of found terms */
    idx_t pow10 = 1;          /* 10^d, starting with 10^1 = 10 after loop */

    for (idx_t d = 1; ; ++d) {
        pow10 *= 10;                      /* compute 10^d */
        idx_t pow10_prev = pow10 / 10;    /* 10^(d‑1) */

        /* lower and upper bounds for s²: 10^d + 10^(d-1) ≤ s² ≤ 2·10^d – 1 */
        idx_t low_val  = pow10 + pow10_prev;
        idx_t high_val = pow10 * 2 - 1;

        /* ceil(sqrt(low_val)) */
        idx_t low_s = isqrt_u64(low_val);
        if (low_s * low_s < low_val) ++low_s;

        /* floor(sqrt(high_val)) */
        idx_t high_s = isqrt_u64(high_val);

        for (idx_t s = low_s; s <= high_s; ++s) {
            idx_t k = s * s - pow10;     /* k = s² – 10^d           */
            ++count;
            if (count == n) return (seq_t)k;
        }
        /* continue with next digit length d+1 */
    }
}
