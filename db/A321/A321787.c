/*
 * A321787  Irregular triangle read by rows: row n gives the prime
 *          factorization of n-th average of twin prime pairs (A014574).
 * https://oeis.org/A321787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* External utilities supplied by the build */
#include "../utils/build_primes.h"
#include "../utils/factor_u64.h"

/* Sequence: a(n) is the exponent of the *k*‑th prime in the prime
   factorisation of the *n*‑th average of twin prime pairs (A014574).
   The sequence is read row‑by‑row: for each average we list the
   exponents of all primes up to and including its largest prime factor.
   For example, the averages are 4,6,12,18,30,… giving the rows
   [2], [1,1], [2,1], [1,2], [1,1,1], [1,1,0,1], …   (see OEIS entry). */

/* Global state – will be initialised lazily on the first call */
static idx_t *prime_list   = NULL;   /* primes up to a chosen limit */
static size_t prime_count   = 0;     /* number of primes in prime_list */
static seq_t *cache         = NULL;   /* flattened sequence of exponents */
static size_t cache_len     = 0;     /* number of entries currently cached */
static size_t cache_cap     = 0;     /* allocated capacity of cache */
static size_t twin_idx      = 0;     /* index of the lower prime in current twin pair */

/* Ensure the cache has room for `need` entries */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Binary search: number of primes in prime_list that are <= x */
static size_t count_primes_leq(idx_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Produce the next row (prime factorisation exponents of the next
   twin‑prime average) and append it to the cache */
static void generate_next_row(void)
{
    /* Advance twin_idx until it points to a lower prime of a twin pair */
    while (twin_idx + 1 < prime_count &&
           prime_list[twin_idx + 1] != prime_list[twin_idx] + 2) {
        ++twin_idx;
    }
    if (twin_idx + 1 >= prime_count) return;   /* no more twins in list */

    uint64_t lower = prime_list[twin_idx];
    uint64_t avg   = lower + 1;                /* (p + p+2)/2 = p+1 */

    /* Factorise the average */
    uint64_t fac_pr[32];
    uint64_t fac_ex[32];
    size_t fac_cnt = 0;
    factor_u64(avg, fac_pr, fac_ex, &fac_cnt);

    /* Largest prime divisor determines how many exponents to output */
    uint64_t pmax = fac_pr[fac_cnt - 1];
    size_t primes_in_row = count_primes_leq(pmax);

    for (size_t i = 0; i < primes_in_row; ++i) {
        uint64_t prime = prime_list[i];
        uint64_t exp = 0;
        for (size_t j = 0; j < fac_cnt; ++j) {
            if (fac_pr[j] == prime) { exp = fac_ex[j]; break; }
            if (fac_pr[j] > prime)    break;
        }
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)exp;
    }

    /* Move past the lower prime of the twin pair for the next call */
    ++twin_idx;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined                               */

    /* Lazy initialisation of the prime list (sufficient for n up to ~10⁴) */
    if (prime_list == NULL) {
        const idx_t LIMIT = 2000000ULL;  /* 2 million primes safely cover >10 000 twin pairs */
        build_primes(LIMIT, &prime_list, &prime_count);
    }

    /* Generate rows until we have at least n exponents cached */
    while (cache_len < n) {
        generate_next_row();
    }

    return cache[n - 1];
}
