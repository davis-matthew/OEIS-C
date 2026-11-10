/*
 * A400980  a(n) = Sum_{j=0..n} A400979(j).
 * https://oeis.org/A400980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} A400979(j), where A400979(j) = max r+s-j
 *            over r!*s! | j!; computed by a sieve plus a two-pointer scan per
 *            j, cached as prefix sums
 * Range:     u8 n <= 92, u16 n <= 7565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9445 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *pref_tab = NULL;
static idx_t pref_done = 0;      /* pref_tab valid for indices 0..pref_done-1 */
static idx_t pref_cap = 0;

static int A400979_value(uint32_t m, const uint32_t *spf, int32_t *e, int32_t *f)
{
    if (m == 0) return 2;
    memset(e, 0, (m + 1) * sizeof(int32_t));
    memset(f, 0, (m + 1) * sizeof(int32_t));
    uint32_t s = 1, blockp = 0;
    int best = 1;
    int retry = 1;
    for (uint32_t k = 1; k + 1 <= m; ++k) {
        uint32_t v = m - k + 1;
        while (v > 1) {
            uint32_t p = spf[v];
            do { v /= p; e[p]++; } while (v % p == 0);
            if (p == blockp) retry = 1;
        }
        if (retry) {
            retry = 0;
            blockp = 0;
            for (;;) {
                uint32_t t = s + 1;
                if (t > m) break;
                uint32_t u = t;
                int ok = 1;
                while (u > 1) {
                    uint32_t p = spf[u];
                    int c = 0;
                    do { u /= p; c++; } while (u % p == 0);
                    if (f[p] + c > e[p]) { ok = 0; blockp = p; break; }
                }
                if (!ok) break;
                u = t;
                while (u > 1) {
                    uint32_t p = spf[u];
                    do { u /= p; f[p]++; } while (u % p == 0);
                }
                s = t;
            }
        }
        int cand = (int)s - (int)k;
        if (cand > best) best = cand;
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n >= pref_done) {
        uint32_t M = (uint32_t)n;
        uint32_t *spf = (uint32_t *)malloc((M + 2) * sizeof(uint32_t));
        for (uint32_t i = 0; i <= M + 1; ++i) spf[i] = i;
        for (uint32_t i = 2; (uint64_t)i * i <= M + 1; ++i)
            if (spf[i] == i)
                for (uint32_t j = i * i; j <= M + 1; j += i)
                    if (spf[j] == j) spf[j] = i;
        if (n + 1 > pref_cap) {
            pref_tab = (uint64_t *)realloc(pref_tab, (n + 1) * sizeof(uint64_t));
            pref_cap = n + 1;
        }
        int32_t *e = (int32_t *)malloc((M + 2) * sizeof(int32_t));
        int32_t *f = (int32_t *)malloc((M + 2) * sizeof(int32_t));
        for (idx_t j = pref_done; j <= n; ++j) {
            uint64_t v = (uint64_t)A400979_value((uint32_t)j, spf, e, f);
            pref_tab[j] = (j ? pref_tab[j - 1] : 0) + v;
        }
        pref_done = n + 1;
        free(e); free(f); free(spf);
    }
    return (seq_t)pref_tab[n];
}
