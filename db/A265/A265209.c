/*
 * A265209  Irregular triangle read by rows in which row n lists the base-3
 *          digits of 2^n, n >= 0.
 * https://oeis.org/A265209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) is the nth element of the sequence formed by writing the
 *            base‑3 representations of 2^k (k ≥ 0) one after another
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Generate rows of 2^k in base 3 on the fly. */
    /* digits are stored little‑endian: digits[0] is least significant. */
    unsigned char digits[512];
    size_t len = 1;               /* length of current number in base 3 */
    digits[0] = 1;                /* 2^0 = 1 in base 3 */
    idx_t position = 0;           /* first global index of the current row */

    for (idx_t k = 0;; ++k) {
        /* Scan the current row. */
        for (size_t j = 0; j < len; ++j) {
            idx_t idx = position + (idx_t)j;
            if (idx == n) {
                return (seq_t)digits[len - 1 - j];   /* most‑significant first */
            }
        }
        position += (idx_t)len;

        /* Prepare next row: multiply the current number by 2. */
        unsigned carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned temp = digits[i] * 2u + carry;
            digits[i] = (unsigned char)(temp % 3u);
            carry = temp / 3u;
        }
        while (carry > 0) {
            if (len >= sizeof(digits)) {
                /* Buffer overflow guard – should not occur for n ≤ 10000. */
                return (seq_t)0;
            }
            digits[len++] = (unsigned char)(carry % 3u);
            carry /= 3u;
        }
    }
}
