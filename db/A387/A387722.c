/*
 * A387722  Numbers k for which A107758(k) > 2*k, but for all whose proper
 *          divisors d|k, d<k, A107758(d) <= 2*d.
 * https://oeis.org/A387722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/sieve. a(n) = n-th k with A107758(k) > 2k such that
 *            A107758(d) <= 2d for every proper divisor d of k; A107758 is
 *            multiplicative with A107758(p^e) = 2 + p + ... + p^e
 * Range:     u8 n <= 32, u16 n <= 3611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9114 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *g_terms = NULL;
static size_t g_cnt = 0;

static void A387722_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint64_t *f = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    unsigned char *bad = (unsigned char *)calloc(L + 1, 1);
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[0] = 0;
    if (L >= 1) f[1] = 1;
    for (i = 2; i <= L; i++) {
        uint64_t p = spf[i], m = i, s = 2, pw = 1;
        while (m % p == 0) {
            m /= p;
            pw *= p;
            s += pw;
        }
        f[i] = s * f[m];
    }
    free(g_terms);
    g_terms = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    g_cnt = 0;
    for (i = 1; i <= L; i++) {
        if (f[i] > 2 * i) {
            if (!bad[i]) g_terms[g_cnt++] = i;
            for (j = 2 * i; j <= L; j += i) bad[j] = 1;
        }
    }
    free(spf);
    free(f);
    free(bad);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (limit == 0) {
        limit = 1024;
        A387722_build(limit);
    }
    while ((uint64_t)n > g_cnt) {
        limit *= 2;
        A387722_build(limit);
    }
    return (seq_t)g_terms[(size_t)(n - 1)];
}
