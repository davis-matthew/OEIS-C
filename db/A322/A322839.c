/*
 * A322839  Numbers n with more prime factors (counted with multiplicity) than
 *          n+1.
 * https://oeis.org/A322839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute the n-th integer m such that Ω(m) > Ω(m+1) by sieving
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* memoized cache of found terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* sieving state */
static idx_t sieve_limit = 0;       /* current maximum number sieved (up to sieve_limit+1) */
static uint32_t *spf = NULL;        /* smallest prime factor for each integer */
static uint8_t *omega = NULL;        /* Ω(n) for each integer */
static idx_t next_to_test = 0;      /* next integer to test after current cache */

/* enlarge the sieve, recomputing spf and omega up to new_limit+1 */
static void expand_limit(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* free old arrays */
    free(spf);
    free(omega);

    size_t sz = (size_t)new_limit + 2;   /* index up to new_limit+1 inclusive */
    spf   = (uint32_t*)malloc(sz * sizeof(uint32_t));
    omega = (uint8_t*)malloc(sz * sizeof(uint8_t));
    if (!spf || !omega) exit(1);

    /* initialise spf */
    for (idx_t i = 0; i <= (idx_t)(new_limit + 1); ++i) spf[i] = (uint32_t)i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= (idx_t)(new_limit + 1); ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= (idx_t)(new_limit + 1); j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* compute Ω(n) using recurrence Ω(n) = Ω(n / spf[n]) + 1 */
    omega[1] = 0;
    for (idx_t i = 2; i <= (idx_t)(new_limit + 1); ++i) {
        omega[i] = omega[i / spf[i]] + 1;
    }

    sieve_limit = new_limit;
}

/* grow the cache array to accommodate at least 'need' entries */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* a(n) returns the n-th integer m with Ω(m) > Ω(m+1) (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    /* initialisation on first call */
    if (next_to_test == 0) {
        expand_limit(100);         /* modest starting limit */
        next_to_test = 2;          /* start testing from 2 */
    }

    /* generate terms until we reach the requested index */
    while (cache_len < n) {
        if (next_to_test > sieve_limit) {
            /* enlarge sieve; double the limit for good coverage */
            expand_limit(sieve_limit * 2 + 100);
        }

        if (omega[next_to_test] > omega[next_to_test + 1]) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_to_test;
        }
        ++next_to_test;
    }

    return cache[n - 1];
}
