/*
 * A299300  Values of k such that A065358(k-1) = 0.
 * https://oeis.org/A299300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k+1 where k is the n-th index (k>=0) with A065358(k)=0,
 *            A065358(m)=Sum_{j=1..m} (-1)^pi(j); computed by sieve with
 *            doubling limit
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *A299300_terms = NULL;
static idx_t A299300_cnt = 0;
static idx_t A299300_lim = 0;

static void A299300_fill(idx_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    idx_t *t = (idx_t *)malloc(sizeof(idx_t) * ((size_t)L / 2 + 2));
    if (!comp || !t) exit(1);
    for (idx_t p = 2; p * p <= L; ++p)
        if (!comp[p])
            for (idx_t j = p * p; j <= L; j += p) comp[j] = 1;
    long long s = 0;
    idx_t pi = 0, c = 0;
    t[c++] = 1; /* m = 0 */
    for (idx_t m = 1; m <= L; ++m) {
        if (m >= 2 && !comp[m]) ++pi;
        s += (pi & 1) ? -1 : 1;
        if (s == 0) t[c++] = m + 1;
    }
    free(comp);
    free(A299300_terms);
    A299300_terms = t;
    A299300_cnt = c;
    A299300_lim = L;
}

seq_t a(idx_t n)
{
    if (A299300_lim == 0) A299300_fill(1 << 16);
    while (n > A299300_cnt) A299300_fill(A299300_lim * 4);
    return (seq_t)A299300_terms[n - 1];
}
