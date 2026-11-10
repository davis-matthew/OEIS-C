/*
 * A324296  Smallest m such that A217157(m) = n.
 * https://oeis.org/A324296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least m such that A217157(m) = n (least k with m^k having
 *            two equal adjacent decimal digits); computed by direct search
 *            over m
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A324296_rep(const uint8_t *d, int len)
{
    for (int i = len - 1; i > 0; --i)
        if (d[i] == d[i - 1]) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 16) return 0;
    const uint64_t LIMIT = 3000000ULL;
    for (uint64_t m = 2; m <= LIMIT; ++m) {
        uint8_t d[256];
        int len = 1;
        d[0] = 1;
        int found = 0;
        for (int k = 1; k <= (int)n; ++k) {
            uint64_t carry = 0;
            for (int i = 0; i < len; ++i) {
                uint64_t p = (uint64_t)d[i] * m + carry;
                d[i] = (uint8_t)(p % 10u);
                carry = p / 10u;
            }
            while (carry > 0) {
                d[len++] = (uint8_t)(carry % 10u);
                carry /= 10u;
            }
            if (A324296_rep(d, len)) {
                found = k;
                break;
            }
        }
        if (found == (int)n) return (seq_t)m;
    }
    return 0;
}
