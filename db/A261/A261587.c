/*
 * A261587  Sum of sexagesimal digits of Fibonacci numbers in base-60
 *          representation.
 * https://oeis.org/A261587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base‑60 digits of the Fibonacci number F(n)
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Maximum number of base‑60 digits needed for n ≤ 10000 is ≈ 1200,
       so we allocate a buffer that is safely large. */
    const int MAX_DIGITS = 2000;
    uint8_t f0[MAX_DIGITS], f1[MAX_DIGITS], f2[MAX_DIGITS];
    int len0 = 1, len1 = 1;
    f0[0] = 0;
    f1[0] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        int carry = 0;
        int maxlen = (len0 > len1) ? len0 : len1;
        int pos = 0;
        for (; pos < maxlen; ++pos) {
            int a = (pos < len0) ? f0[pos] : 0;
            int b = (pos < len1) ? f1[pos] : 0;
            int sum = a + b + carry;
            f2[pos] = (uint8_t)(sum % 60);
            carry = sum / 60;
        }
        if (carry) {
            f2[pos++] = (uint8_t)carry;
        }
        int len2 = pos;

        /* Shift f1 → f0 and f2 → f1 for next iteration. */
        len0 = len1;
        for (pos = 0; pos < len0; ++pos) f0[pos] = f1[pos];
        len1 = len2;
        for (pos = 0; pos < len1; ++pos) f1[pos] = f2[pos];
    }

    seq_t sum = 0;
    for (int i = 0; i < len1; ++i) sum += f1[i];
    return sum;
}
