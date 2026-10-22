/*
 * A362050  Numbers whose prime indices satisfy: (length) = 2*(median).
 * https://oeis.org/A362050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A362050: numbers whose prime‑index multiset has  
 * length equal to twice the median.  The algorithm
 * scans integers in increasing order, factorises them
 * with an incremental SPF sieve, tests the condition
 * and caches the values so that any n ≥ 1 can be
 * retrieved in O(1) after the initial scanning. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  SPF sieve with prime indices (1‑based)                               */
/* --------------------------------------------------------------------- */

static uint32_t *spf = NULL;             /* smallest prime factor */
static uint32_t *prime_idx = NULL;       /* index of a prime (1‑based) */
static uint32_t spf_limit = 0;           /* size of the sieve arrays */
static uint32_t prime_count = 0;         /* number of primes found */

/* Rebuild the SPF and prime_index tables up to `limit`. */
static void rebuild_spf(uint32_t limit)
{
    /* free previous memory */
    free(spf);   free(prime_idx);

    spf        = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    prime_idx  = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf || !prime_idx) abort();

    prime_count = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                       /* i is prime */
            spf[i] = i;
            prime_idx[i] = ++prime_count;
            /* cross‑mark composites */
            if ((uint64_t)i * i <= (uint64_t)limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    if (!spf[(uint32_t)j]) spf[(uint32_t)j] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Grow the sieve only when needed: double the size each time.
 * This keeps the total cost near O(n log log n) for scanning *n
 * numbers. */
static void ensure_spf(uint64_t limit)
{
    if (limit <= (uint64_t)spf_limit) return;
    uint64_t new_limit = spf_limit ? (uint64_t)spf_limit * 2 : 1024ULL;
    if (new_limit < limit) new_limit = limit;
    if (new_limit > 0xFFFFFFFFULL) new_limit = 0xFFFFFFFFULL;  /* avoid overflow */
    rebuild_spf((uint32_t)new_limit);
}

/* --------------------------------------------------------------------- */
/*  Cache of already found terms                                          */
/* --------------------------------------------------------------------- */

static seq_t *cache = NULL;      /* a(1) ... a(cache_size) */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static uint64_t last_scanned = 1;/* last integer examined (last_scanned-1 scanned) */

/* --------------------------------------------------------------------- */
/*  Main accessor                                                        */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – defined as 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        ++last_scanned;
        uint64_t m = last_scanned;
        if (m < 2) continue;      /* skip 1 */

        ensure_spf(m);

        /* collect prime indices (with multiplicity) */
        uint32_t indices[64];
        size_t idx_len = 0;
        uint64_t temp = m;
        while (temp > 1) {
            uint32_t p = spf[(uint32_t)temp];
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint32_t id = prime_idx[p];
            for (uint32_t k = 0; k < exp && idx_len < 64; ++k)
                indices[idx_len++] = id;
        }

        if (idx_len == 0) continue;          /* skip 1 */
        if (idx_len & 1) continue;            /* length must be even */

        /* quick insertion sort (indices are small) */
        for (size_t i = 1; i < idx_len; ++i) {
            uint32_t key = indices[i];
            size_t j = i;
            while (j > 0 && indices[j-1] > key) {
                indices[j] = indices[j-1];
                --j;
            }
            indices[j] = key;
        }

        /* Check that the sum of the two middle indices equals length. */
        size_t k = idx_len / 2;
        if (indices[k-1] + indices[k] != idx_len) continue;

        /* Store m in the cache. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 256;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)m;
    }

    return cache[n-1];
}
