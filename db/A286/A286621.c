/*
 * A286621  Restricted growth sequence computed for filter-sequence A278221,
 *          related to the conjugated prime factorization (see A122111).
 * https://oeis.org/A286621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   restricted growth sequence of the filter A278221(n) =
 *            A046523(A122111(n)). A122111(n) has prime signature equal to the
 *            multiset of gaps between consecutive distinct prime indices of n
 *            (starting from 0), so a(n) is the first-occurrence index of that
 *            gap multiset. Computed by a sieve plus a hash ta...
 * Range:     u8 n <= 478, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9557 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint32_t *g_ids = NULL;
static size_t g_L = 0;

static void A278221_build(size_t L)
{
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *ids = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *koff = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *klen = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *pool = (uint32_t *)malloc(sizeof(uint32_t) * 16 * (L + 1));
    size_t hs = 1;
    while (hs < 2 * (L + 1)) hs <<= 1;
    uint32_t *ht = (uint32_t *)calloc(hs, sizeof(uint32_t));
    if (!spf || !pidx || !ids || !koff || !klen || !pool || !ht) abort();

    uint32_t pc = 0;
    for (size_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            pidx[i] = ++pc;
            for (size_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    uint32_t nclass = 0;
    size_t poolsz = 0;
    for (size_t n = 1; n <= L; ++n) {
        uint32_t gaps[32];
        uint32_t cnt = 0;
        size_t m = n;
        uint32_t prev = 0;
        while (m > 1) {
            uint32_t p = spf[m];
            while (m % p == 0) m /= p;
            uint32_t ix = pidx[p];
            gaps[cnt++] = ix - prev;
            prev = ix;
        }
        for (uint32_t i = 1; i < cnt; ++i) {
            uint32_t v = gaps[i];
            uint32_t j = i;
            while (j > 0 && gaps[j - 1] > v) { gaps[j] = gaps[j - 1]; --j; }
            gaps[j] = v;
        }
        uint64_t h = 1469598103934665603ULL;
        h ^= cnt; h *= 1099511628211ULL;
        for (uint32_t i = 0; i < cnt; ++i) { h ^= gaps[i]; h *= 1099511628211ULL; }
        size_t pos = (size_t)(h ^ (h >> 29)) & (hs - 1);
        uint32_t found = 0;
        while (ht[pos] != 0) {
            uint32_t c = ht[pos];
            if (klen[c] == cnt) {
                uint32_t eq = 1;
                for (uint32_t i = 0; i < cnt; ++i)
                    if (pool[koff[c] + i] != gaps[i]) { eq = 0; break; }
                if (eq) { found = c; break; }
            }
            pos = (pos + 1) & (hs - 1);
        }
        if (!found) {
            found = ++nclass;
            koff[found] = (uint32_t)poolsz;
            klen[found] = cnt;
            for (uint32_t i = 0; i < cnt; ++i) pool[poolsz++] = gaps[i];
            ht[pos] = found;
        }
        ids[n] = found;
    }

    free(spf); free(pidx); free(koff); free(klen); free(pool); free(ht);
    free(g_ids);
    g_ids = ids;
    g_L = L;
}

seq_t a(idx_t n)
{
    if ((size_t)n > g_L) {
        size_t L = (size_t)n;
        if (L < 2 * g_L) L = 2 * g_L;
        if (L < 1024) L = 1024;
        A278221_build(L);
    }
    return (seq_t)g_ids[(size_t)n];
}
