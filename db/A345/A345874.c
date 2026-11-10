/*
 * A345874  a(n) is the number of distinct pairs of positive integers (u, v)
 *          such that A065157(u, v) = n.
 * https://oeis.org/A345874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors u of n such that the binary string of
 *            n parses uniquely as a concatenation of the tokens "0" and
 *            bin(u) (each v is then determined); computed by checking each
 *            divisor u
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A345874_bitlen(uint64_t v)
{
    int l = 0;
    while (v) { l++; v >>= 1; }
    return l;
}

static int A345874_ok(uint64_t n, uint64_t u)
{
    int L = A345874_bitlen(u);
    uint64_t mask = (L >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << L) - 1);
    int pos = A345874_bitlen(n);
    while (pos > 0) {
        if (((n >> (pos - 1)) & 1) == 0) {
            pos--;
        } else {
            if (pos < L) return 0;
            if (((n >> (pos - L)) & mask) == u) pos -= L;
            else return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t d;
    seq_t cnt = 0;
    if (m == 0) return 0;
    for (d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            if (A345874_ok(m, d)) cnt++;
            if (d != m / d) {
                if (A345874_ok(m, m / d)) cnt++;
            }
        }
    }
    return cnt;
}
