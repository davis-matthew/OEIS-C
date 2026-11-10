/*
 * A391947  Numbers k such that A003415(k) == A276085(k) (mod 2310) and
 *          A003415(k+1) == A276085(k+1) (mod 2310), where A003415 is the
 *          arithmetic derivative and A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A391947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003415(k) == A276085(k) (mod 2310) and
 *            A003415(k+1) == A276085(k+1) (mod 2310). Computed by a sieve
 *            (smallest prime factor, with d and s tables mod 2310), rebuilt
 *            with a larger limit when needed
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 386, u64 n <= 386, u128 n <= 386,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 364 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *pairs_terms = NULL;
static size_t pairs_count = 0;
static uint64_t pairs_limit = 0;

static void pairs_build(uint64_t limit)
{
    uint16_t *spf = (uint16_t *)calloc((size_t)limit + 1, sizeof(uint16_t));
    uint16_t *dd = (uint16_t *)malloc(((size_t)limit + 1) * sizeof(uint16_t));
    uint16_t *ss = (uint16_t *)malloc(((size_t)limit + 1) * sizeof(uint16_t));
    uint64_t *terms = NULL;
    size_t cnt = 0, cap = 0;
    uint64_t p, j, k;

    for (p = 2; p * p <= limit; p++) {
        if (spf[p] == 0) {
            for (j = p * p; j <= limit; j += p)
                if (spf[j] == 0)
                    spf[j] = (uint16_t)p;
        }
    }
    dd[0] = 0; ss[0] = 0;
    dd[1] = 0; ss[1] = 0;
    int prevflag = 1; /* k = 1 satisfies the condition */
    for (k = 2; k <= limit; k++) {
        uint64_t q = spf[k] ? spf[k] : k;
        uint64_t m = k / q;
        uint64_t pr;
        if (q == 2) pr = 1;
        else if (q == 3) pr = 2;
        else if (q == 5) pr = 6;
        else if (q == 7) pr = 30;
        else if (q == 11) pr = 210;
        else pr = 0;
        dd[k] = (uint16_t)((m + q * (uint64_t)dd[m]) % 2310);
        ss[k] = (uint16_t)(((uint64_t)ss[m] + pr) % 2310);
        int flag = (dd[k] == ss[k]);
        if (flag && prevflag) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = k - 1;
        }
        prevflag = flag;
    }
    free(spf);
    free(dd);
    free(ss);
    free(pairs_terms);
    pairs_terms = terms;
    pairs_count = cnt;
    pairs_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if (pairs_limit == 0)
        pairs_build((uint64_t)1 << 22);
    while ((uint64_t)n > pairs_count)
        pairs_build(pairs_limit * 2);
    return (seq_t)pairs_terms[n - 1];
}
