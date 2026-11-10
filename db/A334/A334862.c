/*
 * A334862  a(n) = A334097(n) - A064415(n).
 * https://oeis.org/A334862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A334097(n) - A064415(n); both are completely additive,
 *            with A334097(2)=1, A334097(p)=1+A334097((p+1)/2) and
 *            A064415(2)=1, A064415(p)=1+A064415((p-1)/2) for odd primes p
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9866 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A334862_f(idx_t m, int which)
{
    /* which = 0: A334097, which = 1: A064415 */
    if (m <= 1) return (seq_t)0;
    seq_t res = 0;
    idx_t t = m;
    idx_t p = 2;
    while (t > 1) {
        if (p * p > t) p = t;
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            seq_t ap;
            if (p == 2) ap = 1;
            else if (which == 0) ap = (seq_t)1 + A334862_f((p + 1) / 2, 0);
            else ap = (seq_t)1 + A334862_f((p - 1) / 2, 1);
            res += (seq_t)e * ap;
        }
        p += (p == 2) ? 1 : 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    return A334862_f(n, 0) - A334862_f(n, 1);
}
