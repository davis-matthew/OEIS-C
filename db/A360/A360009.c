/*
 * A360009  Numbers whose prime indices have integer mean and integer median.
 * https://oeis.org/A360009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A360009 – numbers whose prime indices have integer
 * mean and integer median, via a simple sieve and incremental search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Global data for the sieve, prime index table, and memoisation       */
/* --------------------------------------------------------------------- */
static uint32_t *spf           = NULL;      /* smallest prime factor up to limit */
static uint32_t *prime_index   = NULL;      /* prime number → its index (1‑based) */
static size_t   spf_limit      = 0;         /* current size of the sieve arrays */

/* Memoised sequence values (cache) */
static seq_t   *cache         = NULL;      /* cached sequence values */
static size_t   cache_size    = 0;          /* number of cached values */
static size_t   cache_cap     = 0;          /* allocated capacity */

/* Next integer to test for acceptance in the sequence */
static idx_t   next_candidate = 2;

/* --------------------------------------------------------------------- */
/*  Utility: extend the sieve to at least 'needed'                         */
/* --------------------------------------------------------------------- */
static void ensure_sieve_limit(idx_t needed)
{
    if ((size_t)needed <= spf_limit) {
        return;
    }

    size_t new_limit = spf_limit ? spf_limit : 2;
    while (new_limit < (size_t)needed)
        new_limit <<= 1;            /* double until we reach the needed size */

    /* Re‑allocate the sieve and prime‑index tables */
    uint32_t *new_spf        = (uint32_t *)realloc(spf,
                                                  (new_limit + 1) * sizeof(uint32_t));
    uint32_t *new_prime_idx  = (uint32_t *)realloc(prime_index,
                                                  (new_limit + 1) * sizeof(uint32_t));
    if (!new_spf || !new_prime_idx) { /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    spf        = new_spf;
    prime_index= new_prime_idx;

    /* Initialise the new region to zero */
    memset(spf, 0, (new_limit + 1) * sizeof(uint32_t));
    memset(prime_index, 0, (new_limit + 1) * sizeof(uint32_t));

    /* Linear sieve to fill spf and prime_index */
    size_t primes_cnt = 0;
    uint32_t *primes_list = (uint32_t *)malloc((new_limit / 2 + 1) * sizeof(uint32_t));
    if (!primes_list) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            prime_index[i] = (uint32_t)(primes_cnt + 1);   /* indices start at 1 */
            primes_list[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes_list[j];
            if (p > spf[i] || (i * (size_t)p) > new_limit)
                break;
            spf[i * (size_t)p] = p;
        }
    }

    free(primes_list);
    spf_limit = new_limit;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {                     /* not defined for n <= 0; return 0 safely */
        return 0;
    }

    /* Allocate the cache if it does not yet exist */
    if (!cache) {
        cache_cap = 32;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Generate sequence entries until we have at least n values */
    while (cache_size < (size_t)n) {
        idx_t current = next_candidate++;
        ensure_sieve_limit(current);

        /* Factorise 'current' using the precomputed SPF array */
        uint32_t idx_arr[128];          /* prime indices */
        uint32_t cnt_arr[128];          /* multiplicities for each prime */
        size_t   sz = 0;
        idx_t tmp = current;

        while (tmp > 1) {
            uint32_t p = spf[(size_t)tmp];
            uint32_t mult = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++mult;
            }
            /* Store the prime index and its multiplicity */
            if (sz >= 128) {            /* safety check – unlikely to happen */
                /* Skip this number if too many distinct primes (rare). */
                sz = 127;
                break;
            }
            idx_arr[sz]  = prime_index[p];
            cnt_arr[sz]  = mult;
            ++sz;
        }

        /* Calculate total multiplicity count and the weighted sum of indices */
        seq_t total   = 0;             /* sum of multiplicities */
        seq_t sum_idx = 0;             /* sum of (index * multiplicity) */

        for (size_t i = 0; i < sz; ++i) {
            total   += (seq_t)cnt_arr[i];
            sum_idx += (seq_t)idx_arr[i] * (seq_t)cnt_arr[i];
        }

        /* Require integer mean: sum_idx / total must be integer */
        if (total == 0 || (sum_idx % total) != 0) {
            continue;                 /* mean not integer → reject */
        }

        /* Median condition */
        bool median_ok = true;
        if (total % 2 == 0) {          /* even number of elements */
            seq_t pos1 = total / 2;    /* 1‑based positions of the two middle elements */
            seq_t pos2 = pos1 + 1;

            seq_t cum = 0;
            uint32_t idx1 = 0, idx2 = 0;
            for (size_t i = 0; i < sz; ++i) {
                cum += (seq_t)cnt_arr[i];
                if (!idx1 && cum >= pos1) idx1 = idx_arr[i];
                if (cum >= pos2) { idx2 = idx_arr[i]; break; }
            }
            median_ok = ((idx1 ^ idx2) & 1) == 0;   /* sum even → integer median */
        }
        /* For odd total, median is always integer */

        if (!median_ok) {
            continue;                 /* reject */
        }

        /* Accept number: add to cache */
        if (!cache || cache_size == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 32;
            seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);
            cache = new_cache;
        }
        cache[cache_size++] = (seq_t)current;
    }

    return cache[n - 1];                /* sequence is 1‑based */
}
