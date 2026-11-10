/*
 * A368920  Number of terms of A368904 less than 10^n, where A368904 gives the
 *          numbers k for which there is no prime p such that p^p divides
 *          A342001(k).
 * https://oeis.org/A368920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k < 10^n : no prime p has p^p | A342001(k) }, where
 *            A342001(k) = rad(k)*Sum_{p^v||k} v/p; computed by a segmented
 *            sieve of rad and A342001 over k = 1..10^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static int A368920_bad(uint64_t v)
{
    if (v == 0) return 1;
    if (v % 4 == 0) return 1;
    if (v % 27 == 0) return 1;
    if (v % 3125 == 0) return 1;
    if (v % 823543 == 0) return 1;
    if (v >= 285311670611ULL) {
        if (v % 285311670611ULL == 0) return 1;
        if (v >= 302875106592253ULL && v % 302875106592253ULL == 0) return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t N = 1;
    for (idx_t i = 0; i < n; i++) N *= 10;
    N -= 1;                                   /* k in [1, N] */
    uint64_t r = 1;
    while (r * r <= N) r++;
    /* primes up to r */
    size_t lim = (size_t)r + 1;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * (lim + 1));
    size_t pc = 0;
    for (size_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            pr[pc++] = i;
            for (size_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    const uint64_t SEG = 1 << 14;
    uint64_t *rad = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
    uint64_t *prod = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
    uint64_t *val = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
    uint64_t count = 0;
    for (uint64_t lo = 2; lo <= N; lo += SEG) {
        uint64_t hi = lo + SEG - 1;
        if (hi > N) hi = N;
        uint64_t len = hi - lo + 1;
        for (uint64_t i = 0; i < len; i++) { rad[i] = 1; prod[i] = 1; val[i] = 0; }
        for (size_t t = 0; t < pc; t++) {
            uint64_t p = pr[t];
            if (p > hi) break;
            uint64_t first = ((lo + p - 1) / p) * p;
            for (uint64_t m = first; m <= hi; m += p) {
                uint64_t i = m - lo;
                val[i] = val[i] * p + rad[i];
                rad[i] *= p;
                prod[i] *= p;
            }
            for (uint64_t pe = p * p; pe <= hi; pe *= p) {
                uint64_t f2 = ((lo + pe - 1) / pe) * pe;
                for (uint64_t m = f2; m <= hi; m += pe) {
                    uint64_t i = m - lo;
                    val[i] += rad[i] / p;
                    prod[i] *= p;
                }
                if (pe > hi / p) break;
            }
        }
        for (uint64_t i = 0; i < len; i++) {
            uint64_t k = lo + i;
            if (prod[i] != k) {
                uint64_t q = k / prod[i];
                val[i] = val[i] * q + rad[i];
            }
            if (!A368920_bad(val[i])) count++;
        }
    }
    free(rad); free(prod); free(val); free(pr); free(comp);
    return (seq_t)count;
}
