/*
 * A304739  a(n) = A056239(A064413(n)).
 * https://oeis.org/A304739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(A064413(n)); computed by building the EKG
 *            sequence with a sieve, then summing prime-index*exponent over
 *            the factorization of the n-th term
 * Range:     u8 n <= 3095, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 121 terms through n = 9382 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *ekg_terms = NULL;
static uint32_t *ekg_spf = NULL;
static uint32_t *ekg_pidx = NULL;
static idx_t ekg_cap = 0;     /* number of terms computed */

static void ekg_build(idx_t n)
{
    size_t bound = (size_t)n * 4 + 100;
    size_t i, j;
    free(ekg_terms); free(ekg_spf); free(ekg_pidx);
    ekg_terms = (uint32_t *)calloc((size_t)n + 2, sizeof(uint32_t));
    ekg_spf = (uint32_t *)calloc(bound + 1, sizeof(uint32_t));
    ekg_pidx = (uint32_t *)calloc(bound + 1, sizeof(uint32_t));
    uint32_t *nxt = (uint32_t *)calloc(bound + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(bound + 1, 1);
    uint32_t cnt = 0;
    for (i = 2; i <= bound; ++i) {
        if (ekg_spf[i] == 0) {
            ekg_spf[i] = (uint32_t)i;
            ekg_pidx[i] = ++cnt;
            nxt[i] = (uint32_t)i;
            for (j = i * i; j <= bound; j += i)
                if (ekg_spf[j] == 0) ekg_spf[j] = (uint32_t)i;
        }
    }
    ekg_terms[1] = 1;
    used[1] = 1;
    if (n >= 2) { ekg_terms[2] = 2; used[2] = 1; }
    for (idx_t k = 3; k <= n; ++k) {
        uint32_t x = ekg_terms[k - 1];
        uint32_t best = UINT32_MAX;
        while (x > 1) {
            uint32_t p = ekg_spf[x];
            while (x % p == 0) x /= p;
            size_t c = nxt[p];
            while (c <= bound && used[c]) c += p;
            nxt[p] = (uint32_t)(c <= bound ? c : bound + 1);
            if (c <= bound && c < best) best = (uint32_t)c;
        }
        ekg_terms[k] = best;
        used[best] = 1;
    }
    free(nxt);
    free(used);
    ekg_cap = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > ekg_cap) {
        idx_t want = ekg_cap * 2;
        if (want < n) want = n;
        if (want < 10000) want = 10000;
        ekg_build(want);
    }
    uint32_t m = ekg_terms[n];
    seq_t sum = 0;
    while (m > 1) {
        uint32_t p = ekg_spf[m];
        uint32_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        sum += (seq_t)ekg_pidx[p] * (seq_t)e;
    }
    return sum;
}
