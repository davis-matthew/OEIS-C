/*
 * A368160  For any n > 0, a(n) is obtained by interpreting the factorial base
 *          expansion of n in base m + 1, where m is the maximum digit in the
 *          factorial base expansion of n; a(0) = 0.
 * https://oeis.org/A368160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by interpreting the factorial‑base expansion of
 *            n in base m+1, where m is the maximum digit in that expansion;
 *            a(0)=0
 * Range:     u8 n <= 95, u16 n <= 4319, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Factorial‑base digits for n: digits[i] corresponds to i! (i starting at 1). */
    uint8_t digits[64];
    int len = 0;
    idx_t temp = n;

    while (temp > 0) {
        idx_t base = (idx_t)(len + 2);            /* next factor: (len+1)+1  */
        uint8_t d = temp % base;                  /* digit for (len+1)!   */
        digits[len++] = d;
        temp /= base;
    }

    /* Determine the maximum digit m. */
    uint8_t m = 0;
    for (int i = 0; i < len; ++i) {
        if (digits[i] > m) m = digits[i];
    }

    /* Interpret the same digits in base (m+1). */
    seq_t base_val = (seq_t)(m + 1);
    seq_t value = 0;
    for (int i = len - 1; i >= 0; --i) {
        value = value * base_val + (seq_t)digits[i];
    }

    return value;
}
