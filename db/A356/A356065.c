/*
 * A356065  Squarefree numbers whose prime indices are all prime-powers.
 * https://oeis.org/A356065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356065 – Squarefree numbers whose prime indices are prime‑powers.
   The function is computed by sieving integers up to a growing bound and
   testing whether each integer satisfies the squarefree condition and all
   its prime factors have an index that is a prime‑power.  The result is
   memoised so that repeated calls are fast. */
   
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Global sieve data ------------------------------------------------- */
static uint64_t *spf = NULL;            /* smallest prime factor for each number */
static uint64_t *primes = NULL;         /* list of primes found so far */
static unsigned char *prime_power_flag = NULL;  /* flag for indices that are prime‑powers */

static size_t primes_cnt = 0;           /* number of primes stored */
static uint64_t sieve_limit = 0;        /* largest number that has been sieved */

/* ------------------------------------------------------------------ */
/* Cache for sequence terms ------------------------------------------ */
static seq_t *cache = NULL;             /* cached sequence values */
static size_t cache_size = 0;           /* number of elements stored */
static size_t cache_cap = 0;            /* allocated capacity */

/* last natural number that has been examined by the sieve */
static uint64_t last_checked = 0;

/* ------------------------------------------------------------------ */
/* Utility: binary search for the (1‑based) index of a prime         */
static size_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;   /* 1‑based index */
        if (primes[mid] < p) lo = mid + 1;
        else                 hi = mid;
    }
    return 0;   /* p not found (should not happen for primes <= sieve_limit) */
}

/* ------------------------------------------------------------------ */
/* Re‑allocate and recompute the sieve up to new_limit                */
static void compute_sieve(uint64_t new_limit)
{
    /* Reallocate the spf and primes arrays if necessary */
    spf    = (uint64_t*)realloc(spf,    (new_limit + 1) * sizeof(uint64_t));
    primes = (uint64_t*)realloc(primes, (new_limit + 1) * sizeof(uint64_t));
    if (!spf || !primes) abort();

    /* Initialise spf array */
    for (uint64_t i = 0; i <= new_limit; ++i) spf[i] = 0;
    primes_cnt = 0;

    /* Linear sieve to fill spf and primes arrays */
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt &&
                          primes[j] <= spf[i] &&
                          i * primes[j] <= new_limit;
             ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    sieve_limit = new_limit;

    /* Build table of indices that are prime‑powers */
    free(prime_power_flag);
    prime_power_flag = (unsigned char*)malloc((primes_cnt + 1) * sizeof(unsigned char));
    if (!prime_power_flag) abort();
    for (size_t k = 1; k <= primes_cnt; ++k) prime_power_flag[k] = 0;
    for (size_t k = 0; k < primes_cnt; ++k) {
        uint64_t p = primes[k];
        uint64_t val = p;
        while (val <= primes_cnt) {
            prime_power_flag[val] = 1;
            if (val > UINT64_MAX / p) break;
            val *= p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Check whether x is squarefree and all its prime factors have indices
   that are prime‑powers                                                */
static bool is_candidate(uint64_t x)
{
    if (x == 1) return true;            /* 1 is considered in the sequence */

    uint64_t n = x;
    while (n > 1) {
        uint64_t p = spf[n];
        if (p == 0) p = n;              /* n is prime > sieve_limit (should not happen) */

        size_t idx = prime_index_of(p);
        if (idx == 0 || !prime_power_flag[idx]) return false;

        n /= p;                         /* remove one occurrence of p */
        if (n % p == 0) return false;   /* square factor detected */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Extend the cached sequence until it contains at least n terms      */
static void ensure_cache(size_t n)
{
    if (cache_size >= n) return;

    /* Initialise sieve if it has not yet been done */
    if (sieve_limit == 0) {
        compute_sieve(1000);
    }

    while (cache_size < n) {
        ++last_checked;
        if (last_checked > sieve_limit) {
            uint64_t new_limit = sieve_limit * 2;
            if (new_limit < last_checked + 100) new_limit = last_checked + 100;
            compute_sieve(new_limit);
        }

        if (is_candidate(last_checked)) {
            /* Grow the cache if necessary */
            if (cache_cap == 0) {
                cache_cap = 128;
                cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
                if (!cache) abort();
            } else if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system                        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; index 0 is undefined */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
