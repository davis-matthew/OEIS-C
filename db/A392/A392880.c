/*
 * A392880  a(n) is the number of iterations of map k -> A392879(k) needed to
 *          reach the fixed point A392878(n), when starting iterating from
 *          k=n.
 * https://oeis.org/A392880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if A392879(n) = n, else 1 + a(A392879(n)); A392879
 *            replaces p^e (p least prime with p^p | n, e = p-adic valuation)
 *            by p^(e mod p) * q^floor(e/p), q = next prime after p
 * Range:     u8 n <= 67500, u16 n <= 67500, u32 n <= 67500, u64 n <= 67500,
 *            u128 n <= 67500, big any n
 * Validated: standard, pass, 124 terms through n = 9682 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const uint64_t ps[6] = {2, 3, 5, 7, 11, 13};
    static const uint64_t qs[6] = {3, 5, 7, 11, 13, 17};
    uint64_t m = (uint64_t)n;
    uint64_t steps = 0;
    if (m == 0) return (seq_t)0;
    for (;;) {
        int found = 0;
        for (int i = 0; i < 6; ++i) {
            uint64_t p = ps[i];
            uint64_t e = 0, t = m;
            while (t % p == 0) { t /= p; ++e; }
            if (e >= p) {
                uint64_t k = e / p;
                uint64_t r = e % p;
                /* t has all p removed; multiply back p^r and q^k */
                uint64_t res = t;
                for (uint64_t j = 0; j < r; ++j) res *= p;
                for (uint64_t j = 0; j < k; ++j) res *= qs[i];
                m = res;
                ++steps;
                found = 1;
                break;
            }
        }
        if (!found) break;
    }
    return (seq_t)steps;
}
