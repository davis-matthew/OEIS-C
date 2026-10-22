/*
 * A373412  Sum of the n-th maximal antirun of nonsquarefree numbers differing
 *          by more than one.
 * https://oeis.org/A373412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the n-th maximal antirun of nonsquarefree
 *            numbers differing by more than one. The sequence is produced by
 *            scanning consecutive nonsquarefree integers, grouping them into
 *            antiruns whenever the difference between consecutive members is
 *            >1, and summing each antirun
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool is_nonsquarefree(uint64_t m, uint32_t *primes, size_t primesCount)
{
    if (m < 4) return false;          /* 1‑3 are squarefree */
    uint64_t temp = m;
    for (size_t i = 0; i < primesCount; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt >= 2) return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static state between calls */
    static uint64_t candidate   = 4;          /* next integer to examine   */
    static uint64_t prev        = 0;          /* previous nonsquarefree int */
    static seq_t   currentSum   = 0;          /* sum of current antirun    */
    static bool    inAntirun    = false;      /* are we inside an antirun? */
    static seq_t *sums         = NULL;        /* array of computed sums    */
    static idx_t   sumCount     = 0;          /* number of sums computed   */
    static idx_t   sumCap       = 0;          /* capacity of sums array    */
    static uint32_t *primes    = NULL;        /* array of small primes     */
    static size_t  primesCount  = 0;          /* number of primes stored   */
    static size_t  primesCap    = 0;          /* capacity of primes array  */
    static size_t  primeLimit   = 0;          /* upper limit for sieve     */

    /* initialise prime table once */
    if (primeLimit == 0) {
        primeLimit = 200000;                 /* sufficient for candidates up to ~1e10 */
        size_t lim = primeLimit;
        uint8_t *isPrime = (uint8_t *)malloc(lim + 1);
        memset(isPrime, 1, lim + 1);
        isPrime[0] = isPrime[1] = 0;
        for (size_t p = 2; p * p <= lim; ++p) {
            if (isPrime[p]) {
                for (size_t m = p * p; m <= lim; m += p) isPrime[m] = 0;
            }
        }
        primesCap = lim / 10 + 10;
        primes = (uint32_t *)malloc(primesCap * sizeof(uint32_t));
        primesCount = 0;
        for (size_t p = 2; p <= lim; ++p) {
            if (isPrime[p]) {
                if (primesCount == primesCap) {
                    primesCap *= 2;
                    primes = (uint32_t *)realloc(primes, primesCap * sizeof(uint32_t));
                }
                primes[primesCount++] = (uint32_t)p;
            }
        }
        free(isPrime);
    }

    /* generate sums until we have at least n of them */
    while (sumCount < n) {
        if (is_nonsquarefree(candidate, primes, primesCount)) {
            if (!inAntirun) {
                currentSum = (seq_t)candidate;
                inAntirun = true;
            } else {
                if (candidate - prev > 1) {
                    currentSum += (seq_t)candidate;
                } else {
                    /* end of current antirun; store its sum */
                    if (sumCap == 0) {
                        sumCap = 16;
                        sums = (seq_t *)malloc(sumCap * sizeof(seq_t));
                    } else if (sumCount == sumCap) {
                        sumCap *= 2;
                        sums = (seq_t *)realloc(sums, sumCap * sizeof(seq_t));
                    }
                    sums[sumCount++] = currentSum;
                    currentSum = (seq_t)candidate;   /* start new antirun */
                }
            }
            prev = candidate;
        }
        ++candidate;
    }

    /* If we exited with an unfinished antirun but no more sums needed, ignore it. */
    return sums[n - 1];
}
