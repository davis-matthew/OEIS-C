/*
 * A270975  Indices i such that A055773(i) is not equal to A130087(i).
 * https://oeis.org/A270975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over i = 1, 2, 3, ...; i is a term when some prime p <=
 *            i/2 has Sum_{j>=1} M(floor(i/p^j)) > 0, where M is the Mertens
 *            function A002321. (The exponent of p in A130087(i) is max(0,
 *            Sum_{j>=1} M(floor(i/p^j))). The exponent of p in A055773(i) is
 *            [i/2 < p <= i]. Primes p > i/2 always agree.)
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int      *A270975_mert = NULL;   /* Mertens function M(0..lim) */
static uint32_t *A270975_pr   = NULL;   /* primes <= lim */
static size_t    A270975_npr  = 0;
static idx_t     A270975_lim  = 0;

static idx_t    *A270975_terms = NULL;
static size_t    A270975_cnt   = 0;
static size_t    A270975_cap   = 0;
static idx_t     A270975_last  = 0;     /* last index i examined */

static void A270975_sieve(idx_t L)
{
    size_t sz = (size_t)L + 1;
    uint32_t *spf = (uint32_t*)calloc(sz, sizeof(uint32_t));
    signed char *mu = (signed char*)calloc(sz, sizeof(signed char));
    size_t pcap = 1024, pn = 0;
    uint32_t *pr = (uint32_t*)malloc(pcap * sizeof(uint32_t));
    if (!spf || !mu || !pr) abort();
    mu[1] = 1;
    for (idx_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            mu[i] = -1;
            if (pn == pcap) {
                pcap *= 2;
                uint32_t *t = (uint32_t*)realloc(pr, pcap * sizeof(uint32_t));
                if (!t) abort();
                pr = t;
            }
            pr[pn++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pn; ++j) {
            idx_t p = pr[j];
            if (p > spf[i] || i * p > L) break;
            spf[i * p] = (uint32_t)p;
            mu[i * p] = (p == spf[i]) ? 0 : (signed char)(-mu[i]);
        }
    }
    int *m = (int*)malloc(sz * sizeof(int));
    if (!m) abort();
    m[0] = 0;
    for (idx_t i = 1; i <= L; ++i) m[i] = m[i - 1] + mu[i];
    free(spf);
    free(mu);
    free(A270975_mert);
    free(A270975_pr);
    A270975_mert = m;
    A270975_pr = pr;
    A270975_npr = pn;
    A270975_lim = L;
}

static bool A270975_differs(idx_t i)
{
    for (size_t k = 0; k < A270975_npr; ++k) {
        idx_t p = A270975_pr[k];
        if (2 * p > i) break;
        long s = 0;
        for (idx_t t = i / p; t > 0; t /= p) s += A270975_mert[t];
        if (s > 0) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A270975_cnt < (size_t)n) {
        idx_t i = A270975_last + 1;
        if (i > A270975_lim) {
            idx_t L = A270975_lim ? A270975_lim * 2 : 4096;
            while (L < i) L *= 2;
            A270975_sieve(L);
        }
        A270975_last = i;
        if (A270975_differs(i)) {
            if (A270975_cnt == A270975_cap) {
                size_t nc = A270975_cap ? A270975_cap * 2 : 64;
                idx_t *t = (idx_t*)realloc(A270975_terms, nc * sizeof(idx_t));
                if (!t) abort();
                A270975_terms = t;
                A270975_cap = nc;
            }
            A270975_terms[A270975_cnt++] = i;
        }
    }
    return (seq_t)A270975_terms[n - 1];
}
