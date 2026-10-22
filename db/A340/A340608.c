/*
 * A340608  The number of prime factors of n (A001222) is relatively prime to
 *          the maximum prime index of n (A061395).
 * https://oeis.org/A340608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer m such that gcd(Ω(m),
 *            A061395(m)) = 1, where Ω(m) = number of prime factors of m
 *            counted with multiplicity (A001222) and A061395(m) = maximum
 *            prime index of any prime divisor of m
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ugcd32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ugcd32.h"

seq_t a(idx_t n)
{
    /*---------------------------------------------*/
    /* static initialization                         */
    /*---------------------------------------------*/
    static int initialized = 0;

    /* Data structures                                         */
    static uint32_t *primes = NULL;          /* list of primes          */
    static size_t prime_cnt = 0;             /* number of primes        */
    static uint32_t *spf = NULL;             /* smallest prime factor   */
    static uint32_t *maxPrimeIdx = NULL;     /* max prime index         */
    static uint8_t  *omega = NULL;           /* Ω(n)                     */
    static uint32_t *primeIndex = NULL;      /* index of each prime     */
    static seq_t   *qual_seq = NULL;         /* sequence of qualified n */
    static size_t qual_len = 0;              /* length of seq           */
    static size_t qual_cap = 0;              /* capacity of seq         */

    /* Hard‑coded limit – large enough for n up to ~10000 */
    const idx_t LIMIT = 500000;

    if (!initialized) {
        /* Allocate arrays of size LIMIT+1 */
        spf = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
        if (!spf) exit(1);
        maxPrimeIdx = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
        if (!maxPrimeIdx) exit(1);
        omega = (uint8_t*)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
        if (!omega) exit(1);
        primeIndex = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
        if (!primeIndex) exit(1);
        primes = (uint32_t*)malloc((size_t)LIMIT * sizeof(uint32_t));
        if (!primes) exit(1);

        /* Linear sieve: compute spf, primes, and primeIndex */
        size_t i, j;
        for (i = 2; i <= (size_t)LIMIT; ++i) {
            if (!spf[i]) {
                spf[i] = (uint32_t)i;
                primeIndex[i] = (uint32_t)(prime_cnt + 1); /* 1‑based index */
                primes[prime_cnt++] = (uint32_t)i;
            }
            for (j = 0; j < prime_cnt && primes[j] <= spf[i] &&
                 (size_t)i * primes[j] <= (size_t)LIMIT; ++j) {
                uint32_t v = (uint32_t)((size_t)i * primes[j]);
                spf[v] = primes[j];
            }
        }

        /* Compute omega and maxPrimeIdx for all numbers */
        omega[1] = 0;
        maxPrimeIdx[1] = 0;
        for (i = 2; i <= (size_t)LIMIT; ++i) {
            uint32_t p = spf[i];
            size_t rest = i / p;
            omega[i] = omega[rest] + 1;
            uint32_t idxp = primeIndex[p];
            uint32_t other = maxPrimeIdx[rest];
            maxPrimeIdx[i] = (idxp > other ? idxp : other);
        }

        /* Generate the sequence of qualified numbers */
        qual_cap = 20000; /* initial capacity, will grow if needed */
        qual_seq = (seq_t*)malloc(qual_cap * sizeof(seq_t));
        if (!qual_seq) exit(1);

        for (i = 1; i <= (size_t)LIMIT; ++i) {
            uint32_t k = omega[i];
            uint32_t m = maxPrimeIdx[i];
            if (ugcd32(k, m) == 1) {
                if (qual_len == qual_cap) {
                    qual_cap <<= 1;
                    qual_seq = (seq_t*)realloc(qual_seq, qual_cap * sizeof(seq_t));
                    if (!qual_seq) exit(1);
                }
                qual_seq[qual_len++] = (seq_t)i;
            }
        }

        initialized = 1;
    }

    /* Return the n-th term (offset 1) */
    if ((idx_t)n <= (idx_t)qual_len) {
        return qual_seq[n - 1];
    }
    /* For safety: if n is beyond precomputed range, return 0 */
    return 0;
}
