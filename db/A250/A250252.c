/*
 * A250252  Inverse permutation to A114881.
 * https://oeis.org/A250252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (((x+y)^2) - x - 3*y)/2, where x = A078898(n+1) and y
 *            = A055396(n+1); computed as a(n) = C(x+y,2) - y + 1
 * Range:     u8 n <= 44, u16 n <= 722, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 92 terms through n = 7881 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t N = n + 1;
    /* smallest prime factor p of N */
    idx_t p = N;
    for (idx_t d = 2; d * d <= N; ++d) {
        if (N % d == 0) { p = d; break; }
    }
    /* x = number of m <= N with smallest prime factor p */
    idx_t x = 0;
    for (idx_t m = p; m <= N; m += p) {
        bool ok = true;
        for (idx_t d = 2; d < p && d * d <= m; ++d) {
            if (m % d == 0) { ok = false; break; }
        }
        if (ok) ++x;
    }
    /* y = index of p among primes (2 is first) */
    idx_t y = 0;
    {
        unsigned char *sv = (unsigned char *)malloc((size_t)p + 1);
        if (!sv) abort();
        for (idx_t i = 0; i <= p; ++i) sv[i] = 1;
        for (idx_t i = 2; i <= p; ++i) {
            if (sv[i]) {
                ++y;
                for (idx_t j = i * i; j <= p && j > i; j += i) sv[j] = 0;
            }
        }
        free(sv);
    }
    /* (s^2 - x - 3y)/2 = s(s-1)/2 - y, with s = x + y */
    idx_t s = x + y;
    idx_t u = s, v = s - 1;
    if ((u & 1) == 0) u /= 2; else v /= 2;
    seq_t t = (seq_t)u * (seq_t)v;
    return t - (seq_t)y + 1;
}
