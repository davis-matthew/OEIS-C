/*
 * A326778  a(n) is the least k such that A213636(k) = n.
 * https://oeis.org/A326778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A213636(k) = n, found by search: k =
 *            lcm(1..m-1)*j, m a prime power > n, k mod m = n; minimize over m
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    seq_t L = 1;
    seq_t best = 0;
    bool have = false;
    for (unsigned long long m = 2; m <= 2 * nn + 2; m++) {
        /* L = lcm(1..m-1) here */
        if (have && L >= best) break;
        if (m > nn) {
            unsigned long long t = m, p = 0;
            for (unsigned long long d = 2; d * d <= t; d++) {
                if (t % d == 0) { p = d; break; }
            }
            if (p == 0) p = m;
            unsigned long long r = m;
            while (r % p == 0) r /= p;
            if (r == 1) {
                unsigned long long pe1 = m / p;
                if (nn % pe1 == 0) {
                    unsigned long long Lm = (unsigned long long)(L % (seq_t)m);
                    for (unsigned long long j = 1; j <= m; j++) {
                        if ((Lm * j) % m == nn) {
                            seq_t cand = L * (seq_t)j;
                            if (cand / (seq_t)j == L) {
                                if (!have || cand < best) { best = cand; have = true; }
                            }
                            break;
                        }
                    }
                }
            }
        }
        /* L = lcm(L, m) with overflow check */
        seq_t x = L, y = (seq_t)m;
        while (y) { seq_t t = x % y; x = y; y = t; }
        seq_t q = L / x;
        seq_t nl = q * (seq_t)m;
        if (nl / (seq_t)m != q) break;
        L = nl;
    }
    return best;
}
