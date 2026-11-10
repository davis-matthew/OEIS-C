/*
 * A348979  Denominator of ratio A332993(n) / sigma(n).
 * https://oeis.org/A348979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), A332993(n)), where A332993(1) =
 *            1 and A332993(n) = n + A332993(n / spf(n)) for n > 1
 * Range:     u8 n <= 139, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 8978 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static idx_t A348979_spf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t p = 3; p <= n / p; p += 2)
        if (n % p == 0) return p;
    return n;
}

static seq_t A348979_chain(idx_t n)
{
    seq_t s = 1;
    idx_t cur = n;
    while (cur > 1) {
        s += (seq_t)cur;
        cur = cur / A348979_spf(cur);
    }
    return s;
}

static seq_t A348979_sigma(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}

seq_t a(idx_t n)
{
    seq_t sg = A348979_sigma(n);
    seq_t ch = A348979_chain(n);
    seq_t g = gcd(sg, ch);
    return sg / g;
}
