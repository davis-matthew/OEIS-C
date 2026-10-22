/*
 * A345960  Numbers whose prime indices have alternating sum 2.
 * https://oeis.org/A345960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by searching for numbers whose prime indices have
 *            alternating sum 2
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------------------------------------------------------------------- */
/* Global data for the sieve: smallest prime factor and prime index.     */
/* ---------------------------------------------------------------------- */
static uint32_t *spf   = NULL;   /* spf[i] = smallest prime factor of i           */
static uint32_t *pidx  = NULL;   /* pidx[p] = index of prime p (1‑based)          */
static size_t    limit = 0;      /* size of the sieve arrays                     */

/* Build a linear sieve up to a fixed size large enough for n ≤ 10000.    */
static void build_sieve(void)
{
    if (spf != NULL)   /* already built */
        return;

    /* Choose a limit that comfortably contains the first ~10000 terms. */
    limit = 20000000ULL;   /* 20 million */

    spf  = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    pidx = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf || !pidx)
        exit(EXIT_FAILURE);

    /* Temporary array for the list of primes. */
    uint32_t *pr = (uint32_t *)malloc(limit * sizeof(uint32_t));
    if (!pr) {
        free(spf);
        free(pidx);
        exit(EXIT_FAILURE);
    }

    size_t pc = 0;  /* number of primes found so far */
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i]  = i;
            pidx[i] = ++pc;
            pr[pc - 1] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = pr[j];
            uint64_t mult = (uint64_t)i * p;
            if (mult > limit)
                break;
            spf[(size_t)mult] = p;
            if (i % p == 0)
                break;
        }
    }
    free(pr);
}

/* ---------------------------------------------------------------------- */
/* Cache for sequence values.                                             */
/* ---------------------------------------------------------------------- */
static seq_t *cache     = NULL;   /* array holding the found terms        */
static size_t cache_len = 0;      /* number of elements stored in cache  */
static size_t cache_cap = 0;      /* capacity of the cache array         */
static idx_t  candidate = 2;      /* next integer to test                */

/* Grow the cache array to accommodate more elements. */
static void grow_cache(void)
{
    if (cache_len < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/* Return the nth term (n ≥ 1) of A345960.                                 */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    build_sieve();

    while (cache_len < (size_t)n) {
        idx_t x = candidate;
        idx_t temp = x;

        int64_t alt_sum = 0;   /* alternating sum of prime indices        */
        int    sign = 1;       /* current sign (+1 or –1)                  */

        /* Factorise x using the precomputed smallest prime factors. */
        while (temp > 1) {
            uint32_t p = spf[(size_t)temp];
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            uint32_t idx = pidx[p];   /* index of prime p             */
            if (e & 1U) {             /* odd exponent: contributes      */
                alt_sum += (int64_t)sign * idx;
                sign = -sign;          /* flip sign for next copy       */
            }
            /* Even exponent: no contribution, sign unchanged. */
        }

        if (alt_sum == 2) {
            grow_cache();
            cache[cache_len++] = (seq_t)x;
        }

        ++candidate;
        /* The sieve limit is large enough for the required n.     */
    }

    return cache[n - 1];
}
