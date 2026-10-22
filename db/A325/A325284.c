/*
 * A325284  Numbers whose prime indices form an initial interval with a single
 *          hole: (1, 2, ..., x, x + 2, ..., m - 1, m), where x can be 0 but
 *          must be less than m - 1.
 * https://oeis.org/A325284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 (NOT USED: this sequence is defined by prime
 *            indices)
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper prototypes (assumed to be available) */
/* #include "../utils/....h"          (none needed for this implementation) */

/* Function to compute the n-th term of OEIS A325284 */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------- */
    /* static initialization of the smallest prime factor array */
    /* --------------------------------------------------------- */
    static bool initialized = false;
    static uint32_t *spf       = NULL;      /* smallest prime factor */
    static uint32_t *primeIdx  = NULL;      /* mapping prime -> its index (1-based) */
    static int      *primes    = NULL;      /* list of primes (optional) */
    static size_t   primeCnt   = 0;
    static size_t   limit      = 5000000;   /* sieve limit – should be enough for n <= 10000 */

    if (!initialized) {
        /* allocate arrays */
        spf      = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
        primeIdx = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
        primes   = (int*)malloc((limit / 2) * sizeof(int));

        /* initialize spf */
        for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;

        /* sieve for smallest prime factor */
        for (size_t i = 2; i * i <= limit; ++i) {
            if (spf[i] == (uint32_t)i) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
                }
            }
        }

        /* build prime list and index mapping */
        primeCnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (spf[i] == (uint32_t)i) {            /* i is prime */
                primeIdx[(uint32_t)i] = (uint32_t)(++primeCnt);   /* 1‑based index */
                primes[primeCnt - 1]  = (int)i;
            }
        }

        initialized = true;
    }

    /* --------------------------------------------------------- */
    /* search for the n-th term */
    /* --------------------------------------------------------- */
    idx_t found = 0;
    for (uint32_t val = 2; val <= limit; ++val) {
        /* factorize val using spf and collect distinct prime indices */
        uint32_t tmp = val;
        uint32_t lastIdx = 0;
        uint32_t indices[32];   /* maximum distinct prime indices for numbers <= 5e6 is small */
        uint8_t  cnt = 0;

        while (tmp > 1) {
            uint32_t p = spf[tmp];
            uint32_t idx = primeIdx[p];
            if (idx != lastIdx) {            /* new distinct prime */
                if (cnt < sizeof(indices)/sizeof(indices[0])) {
                    indices[cnt++] = idx;
                }
                lastIdx = idx;
            }
            /* remove all copies of this prime */
            while (tmp % p == 0) tmp /= p;
        }

        if (cnt == 0) continue;   /* should not happen for val >= 2 */

        uint32_t m = indices[cnt - 1];   /* maximum index in this factorization */
        if (cnt == m - 1) {              /* exactly one index missing between 1..m */
            ++found;
            if (found == n) {
                return (seq_t)val;
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /* Fallback: if limit insufficient (very unlikely for n <= 10000),    */
    /* try extending the sieved range and continue.                       */
    /* ------------------------------------------------------------------ */
    while (found < n) {
        /* extend sieve by doubling the limit */
        size_t oldLimit = limit;
        limit <<= 1;
        spf = (uint32_t*)realloc(spf, (limit + 1) * sizeof(uint32_t));
        primeIdx = (uint32_t*)realloc(primeIdx, (limit + 1) * sizeof(uint32_t));
        primes = (int*)realloc(primes, (limit / 2) * sizeof(int));

        /* initialize new region of spf */
        for (size_t i = oldLimit + 1; i <= limit; ++i) spf[i] = (uint32_t)i;

        /* sieve new region */
        for (size_t i = 2; i * i <= limit; ++i) {
            if (spf[i] == (uint32_t)i) {
                size_t start = i * i;
                if (start < oldLimit + 1) start = oldLimit + 1;
                for (size_t j = start; j <= limit; j += i) {
                    if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
                }
            }
        }

        /* continue searching from oldLimit+1 */
        for (uint32_t val = oldLimit + 1; val <= limit; ++val) {
            uint32_t tmp = val;
            uint32_t lastIdx = 0;
            uint32_t indices[32];
            uint8_t cnt = 0;

            while (tmp > 1) {
                uint32_t p = spf[tmp];
                uint32_t idx = primeIdx[p];
                if (idx != lastIdx) {
                    if (cnt < sizeof(indices)/sizeof(indices[0]))
                        indices[cnt++] = idx;
                    lastIdx = idx;
                }
                while (tmp % p == 0) tmp /= p;
            }
            if (cnt == 0) continue;
            uint32_t m = indices[cnt - 1];
            if (cnt == m - 1) {
                ++found;
                if (found == n) return (seq_t)val;
            }
        }
    }

    /* Should never reach this point */
    return (seq_t)0;
}
