/*
 * A307825  Number of partitions of n into 3 distinct prime powers (not
 *          including 1).
 * https://oeis.org/A307825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into 3 distinct prime
 *            powers (>1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External linkage as required by the build system. */
seq_t a(idx_t n)
{
    /* The smallest possible sum of three distinct prime powers (2+3+4) is 9. */
    if (n < 9) return 0;

    idx_t lim = n;
    size_t sz = (size_t)(lim + 1);

    /* Arrays for the sieve and for marking prime powers. */
    uint8_t *isPrimePower = (uint8_t *)calloc(sz, 1);
    if (!isPrimePower) return 0;

    uint8_t *isComposite = (uint8_t *)calloc(sz, 1);
    if (!isComposite) { free(isPrimePower); return 0; }

    idx_t *primeList = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!primeList) { free(isPrimePower); free(isComposite); return 0; }

    /* Sieve of Eratosthenes to generate primes up to n. */
    size_t primeCnt = 0;
    for (idx_t i = 2; i <= lim; ++i) {
        if (!isComposite[i]) {
            primeList[primeCnt++] = i;
            if (i <= lim / i) {
                for (idx_t j = i * i; j <= lim; j += i) {
                    isComposite[j] = 1;
                }
            }
        }
    }

    /* Mark all prime powers up to n. */
    for (size_t pi = 0; pi < primeCnt; ++pi) {
        idx_t p = primeList[pi];
        idx_t val = p;
        while (1) {
            if (val > lim) break;
            isPrimePower[val] = 1;
            if (val > lim / p) break;
            val *= p;
        }
    }

    /* Collect prime powers into an array for easy iteration. */
    idx_t *pp = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!pp) { free(isPrimePower); free(isComposite); free(primeList); return 0; }
    size_t ppCnt = 0;
    for (idx_t i = 2; i <= lim; ++i) {
        if (isPrimePower[i]) pp[ppCnt++] = i;
    }

    /* Count unordered distinct triples a<b<c with a+b+c = n. */
    seq_t count = 0;
    for (size_t ia = 0; ia < ppCnt; ++ia) {
        idx_t a_val = pp[ia];
        for (size_t ib = ia + 1; ib < ppCnt; ++ib) {
            idx_t b_val = pp[ib];
            idx_t sum_ab = a_val + b_val;
            if (sum_ab > lim) break;
            idx_t c_val = lim - sum_ab;
            if (c_val <= b_val) continue;        /* ensure distinctness */
            if (c_val <= lim && isPrimePower[c_val]) {
                ++count;                          /* count one partition */
            }
        }
    }

    /* Free allocated memory. */
    free(isPrimePower);
    free(isComposite);
    free(primeList);
    free(pp);

    return count;
}
