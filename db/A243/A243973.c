/*
 * A243973  a(n) is the repeating digit in 2^A243972(n).
 * https://oeis.org/A243973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest digit occurring exactly n times in 2^k, where k
 *            = A243972(n) is the least k for which 2^k contains a digit
 *            occurring exactly n times; computed by repeated doubling of the
 *            decimal expansion
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    static uint32_t seen[200001];
    static uint8_t dig[200001];
    static uint8_t digits[20000];
    static size_t len = 1;
    static uint32_t cur_k = 0;
    static int initialized = 0;

    if (!initialized) {
        memset(seen, 0, sizeof(seen));
        memset(dig, 0, sizeof(dig));
        digits[0] = 1;
        len = 1;
        cur_k = 0;
        initialized = 1;
    }

    if (n == 0 || (size_t)n >= sizeof(seen) / sizeof(seen[0]))
        return 0;

    while (seen[n] == 0) {
        unsigned freq[10];
        int carry = 0;
        cur_k++;
        for (size_t i = 0; i < len; ++i) {
            int v = digits[i] * 2 + carry;
            digits[i] = (uint8_t)(v % 10);
            carry = v / 10;
        }
        while (carry) {
            if (len >= sizeof(digits)) return 0;
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
        memset(freq, 0, sizeof(freq));
        for (size_t i = 0; i < len; ++i)
            ++freq[digits[i]];
        for (int d = 0; d < 10; ++d) {
            unsigned f = freq[d];
            if (f > 0 && f < sizeof(seen) / sizeof(seen[0]) && seen[f] == 0) {
                seen[f] = cur_k;
                dig[f] = (uint8_t)d;
            }
        }
    }
    return (seq_t)dig[n];
}
