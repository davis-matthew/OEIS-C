/*
 * A344767  Möbius transform of A011772.
 * https://oeis.org/A344767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * A011772(d), where A011772(d) is the
 *            smallest m >= 1 with d | m(m+1)/2 (found by search)
 * Range:     i8 n <= 127, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9965 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A008683_mu(idx_t n)
{
    if (n == 1) return 1;
    idx_t tmp = n;
    int cnt = 0;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (tmp % p == 0) return 0;
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static idx_t A011772_val(idx_t n)
{
    idx_t limit = n * 2u;
    unsigned __int128 mod = (unsigned __int128)limit;
    for (idx_t m = 1; m < limit; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * (m + 1);
        if (prod % mod == 0) return m;
    }
    return limit - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        int mu = A008683_mu(e);          /* mu(n/d) paired with A011772(d) */
        if (mu != 0) total += (seq_t)mu * (seq_t)A011772_val(d);
        if (e != d) {
            mu = A008683_mu(d);          /* mu(n/e) = mu(d) paired with A011772(e) */
            if (mu != 0) total += (seq_t)mu * (seq_t)A011772_val(e);
        }
    }
    return total;
}
