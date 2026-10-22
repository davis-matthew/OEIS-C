/*
 * A362982  Heinz numbers of partitions such that 2*(least part) < greatest
 *          part.
 * https://oeis.org/A362982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Heinz numbers of partitions with 2*(least part) < greatest part
   a(n) is the n-th smallest positive integer that can be expressed as a product
   of prime indices of a partition satisfying 2*min_index < max_index. */
#include <stdlib.h>
#include <stdint.h>

/* Static data for sieve and cache */
#define LIMIT 2000000      /* Enough for the first 10000 terms */

static uint32_t *spf = NULL;          /* smallest prime factor for each number */
static uint32_t *prime_index = NULL;  /* prime index of each prime number */
static size_t prime_count = 0;

static seq_t *cache = NULL;   /* cache of found terms */
static size_t cache_size = 0;
static size_t last_checked = 1;   /* last integer examined */

static int initialized = 0;

/* Helper: initialize sieve and prime index tables */
static void init_sieve(void)
{
    size_t i, j;
    spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    prime_index = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));

    for (i = 0; i <= LIMIT; ++i) {
        spf[i] = (uint32_t)i;
        prime_index[i] = 0;
    }

    /* Sieve of Eratosthenes with spf */
    for (i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {            /* i is prime */
            if (i * i <= LIMIT) {
                for (j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Assign prime indices (1‑based) */
    prime_count = 0;
    for (i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {            /* i is prime */
            ++prime_count;
            prime_index[i] = (uint32_t)prime_count;
        }
    }
}

seq_t a(idx_t n)
{
    /* First call: set up data structures */
    if (!initialized) {
        initialized = 1;
        init_sieve();
    }

    /* Extend cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        ++last_checked;                      /* next integer to test */

        /* Factorize last_checked using spf */
        uint32_t value = (uint32_t)last_checked;
        uint32_t temp = value;
        uint32_t min_idx = 0, max_idx = 0;
        uint32_t distinct = 0;

        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t idx = prime_index[p];
            if (idx) {
                if (distinct == 0 || idx < min_idx) min_idx = idx;
                if (distinct == 0 || idx > max_idx)   max_idx = idx;
                ++distinct;
            }
            while (temp % p == 0) temp /= p;
        }

        /* Check the condition: at least two distinct primes and max>2*min */
        if (distinct >= 2 && max_idx > 2 * min_idx) {
            cache = (seq_t*)realloc(cache,
                                     (cache_size + 1) * sizeof(seq_t));
            cache[cache_size++] = (seq_t)value;
        }
    }

    return cache[n - 1];
}
