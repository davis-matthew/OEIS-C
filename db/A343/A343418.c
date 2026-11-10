/*
 * A343418  Primes that occur in A343416.
 * https://oeis.org/A343418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest prime that equals A343416(m) for some m,
 *            where A343416(m) =
 *            A001414(m)+A000203(m)+A001414(A000203(m))+A000203(A001414(m)).
 *            Computed by sieve up to a limit X (A343416(m) > m), doubling X
 *            until n primes are found
 * Range:     u8 n <= 21, u16 n <= 2731, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Validated: standard, pass, 77 terms through n = 3657 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A343418_cache = NULL;
static size_t A343418_cnt = 0;

static void A343418_build(uint64_t X)
{
    uint64_t i, j;
    uint64_t *sig = (uint64_t *)calloc((size_t)(X + 1), sizeof(uint64_t));
    uint64_t *sop = (uint64_t *)calloc((size_t)(X + 1), sizeof(uint64_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)(X + 1), 1);
    unsigned char *mark = (unsigned char *)calloc((size_t)(X + 1), 1);
    for (i = 1; i <= X; i++)
        for (j = i; j <= X; j += i) sig[j] += i;
    /* sieve and sopfr */
    for (i = 2; i <= X; i++) {
        if (!comp[i]) {
            for (j = i; j <= X; j += i) {
                if (j > i) comp[j] = 1;
            }
        }
    }
    for (i = 2; i <= X; i++) {
        if (!comp[i]) {
            sop[i] = i;
        }
    }
    /* sopfr via smallest prime factor */
    {
        uint64_t *spf = (uint64_t *)calloc((size_t)(X + 1), sizeof(uint64_t));
        for (i = 2; i <= X; i++) {
            if (spf[i] == 0) {
                for (j = i; j <= X; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
        for (i = 2; i <= X; i++) sop[i] = sop[i / spf[i]] + spf[i];
        free(spf);
    }
    for (i = 1; i <= X; i++) {
        uint64_t s = sig[i];
        if (s > X) continue;
        uint64_t v = sop[i] + s + sop[s] + sig[sop[i]];
        if (v >= 2 && v <= X && !comp[v]) mark[v] = 1;
    }
    size_t c = 0;
    for (i = 2; i <= X; i++) if (mark[i]) c++;
    free(A343418_cache);
    A343418_cache = (uint64_t *)malloc((c + 1) * sizeof(uint64_t));
    A343418_cnt = 0;
    for (i = 2; i <= X; i++) if (mark[i]) A343418_cache[A343418_cnt++] = i;
    free(sig); free(sop); free(comp); free(mark);
}

extern seq_t a(idx_t n)
{
    static uint64_t X = 0;
    if (n < 1) return (seq_t)0;
    while (X == 0 || A343418_cnt < (size_t)n) {
        X = (X == 0) ? 4096 : X * 2;
        A343418_build(X);
    }
    return (seq_t)A343418_cache[(size_t)(n - 1)];
}
