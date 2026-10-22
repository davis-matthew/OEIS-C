/*
 * A319848  Irregular triangle read by rows in which row n lists the even
 *          divisors of n in decreasing order together with the odd divisors
 *          of n in decreasing order.
 * https://oeis.org/A319848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   cmp_desc_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th element of the irregular triangle obtained by,
   for each r ≥ 1, listing the even divisors of r in decreasing
   order followed by the odd divisors of r in decreasing order.
   The function generates rows incrementally until the cumulative
   number of elements reaches n, then returns the element at that
   position. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp_desc_uint64.h"

/* factorise x into primes[] and exps[].  *cnt receives the number of
   distinct prime factors.  Return the total divisor count. */
static size_t factorise(idx_t x, uint64_t *primes, int *exps, size_t *cnt)
{
    size_t dcnt = 1;
    size_t pc = 0;
    uint64_t n = (uint64_t)x;
    /* factor 2 */
    int e = 0;
    while ((n & 1ULL) == 0ULL) {
        n >>= 1U;
        ++e;
    }
    if (e) {
        primes[pc] = 2ULL;
        exps[pc] = e;
        ++pc;
        dcnt *= (size_t)(e + 1);
    }
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p) continue;
        int e2 = 0;
        while (n % p == 0ULL) {
            n /= p;
            ++e2;
        }
        primes[pc] = p;
        exps[pc] = e2;
        ++pc;
        dcnt *= (size_t)(e2 + 1);
    }
    if (n > 1ULL) {
        primes[pc] = n;
        exps[pc] = 1;
        ++pc;
        dcnt *= 2U;
    }
    *cnt = pc;
    return dcnt;
}

/* generate all divisors of x given its factorisation.  The array
   divs must have space for at least dcnt elements.  On return
   *dcnt is the number of divisors written. */
static void generate_divisors(idx_t x,
                              const uint64_t *primes,
                              const int *exps,
                              size_t pfcnt,
                              idx_t *divs,
                              size_t *dcnt)
{
    divs[0] = 1ULL;
    *dcnt = 1;
    for (size_t i = 0; i < pfcnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t cur = *dcnt;
        uint64_t pow = 1ULL;
        for (int k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < cur; ++j) {
                divs[(*dcnt)++] = divs[j] * pow;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for zero index */

    /* locate the row r such that cumulative divisor count up to r
       exceeds or equals n. */
    idx_t r = 1;
    idx_t cum = 0;
    uint64_t primes[64];
    int exps[64];
    size_t pfcnt, dcnt;
    while (1) {
        dcnt = factorise(r, primes, exps, &pfcnt);
        if (cum + (idx_t)dcnt >= n) break;
        cum += (idx_t)dcnt;
        ++r;
    }

    /* position within the row (1‑based) */
    idx_t pos = n - cum;

    /* allocate array for divisors */
    idx_t *divs = (idx_t *)malloc(dcnt * sizeof(idx_t));
    if (!divs) return 0; /* out‑of‑memory protection */
    size_t divcnt;
    generate_divisors(r, primes, exps, pfcnt, divs, &divcnt);

    /* separate even and odd divisors */
    idx_t *evens = (idx_t *)malloc(divcnt * sizeof(idx_t));
    idx_t *odds  = (idx_t *)malloc(divcnt * sizeof(idx_t));
    if (!evens || !odds) {
        free(divs);
        free(evens); free(odds);
        return 0;
    }
    size_t evenc = 0, oddc = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        if ((divs[i] & 1ULL) == 0ULL)
            evens[evenc++] = divs[i];
        else
            odds[oddc++] = divs[i];
    }

    /* sort each group in decreasing order */
    qsort(evens, evenc, sizeof(idx_t), cmp_desc_uint64);
    qsort(odds, oddc, sizeof(idx_t), cmp_desc_uint64);

    /* extract the desired element */
    idx_t result;
    if (pos <= evenc)
        result = evens[pos - 1];
    else
        result = odds[pos - evenc - 1];

    free(divs);
    free(evens);
    free(odds);
    return (seq_t)result;
}
