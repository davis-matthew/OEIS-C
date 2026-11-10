/*
 * A344765  a(n) = sigma(n) - A011772(n).
 * https://oeis.org/A344765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - A011772(n), where A011772(n) is found by
 *            search for the least m with n | m(m+1)/2
 * Range:     u8 n <= 119, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9308 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344765_sigma(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

static seq_t A344765_A011772(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t limit = N * 2;
    for (seq_t m = 1; m < limit; ++m) {
        seq_t x = m, y = m + 1;
        if (x % 2 == 0) x /= 2; else y /= 2;
        if (((x % N) * (y % N)) % N == 0)
            return m;
    }
    return limit - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return A344765_sigma(n) - A344765_A011772(n);
}
