/*
 * A260684  Irregular triangular array read by rows. Row n gives the primes in
 *          the prime factorization of n! that have exponent of 1.
 * https://oeis.org/A260684
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the nth term obtained by reading rows of primes p ≤ r
 *            whose exponent in r! is exactly 1. For any row r, the wanted
 *            primes are exactly the primes with r/2 < p ≤ r, i.e., the primes
 *            in (r/2 , r]. The sequence a(n) is produced by writing each of
 *            these rows in order and reading the entries fro...
 * Range:     u8 n <= 3294, u16 n <= 9658, u32 n <= 9658, u64 n <= 9658,
 *            u128 n <= 9658, big any n
 * Helpers:   build_primes_upto, prime_first_ge, prime_first_gt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */
#include "../utils/prime_first_ge.h"
#include "../utils/prime_first_gt.h"

/* Global state used for memoisation */
static seq_t *cache      = NULL;   /* cache[n] = a(n) */
static idx_t  cache_cap  = 0;       /* allocated capacity of cache */
static idx_t  cache_size = 0;       /* next free index in cache (initially 2) */

static uint64_t *prime_arr = NULL;  /* array of primes, sorted */
static size_t   prime_cnt   = 0;     /* count of primes in prime_arr */
static idx_t    prime_limit = 0;     /* largest prime index generated so far */

static idx_t last_row = 1;          /* rows processed up to last_row (initially 1) */

/* Ensure cache contains a(n) for all n ≤ target */
static void ensure_cache(idx_t target)
{
    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap  = 8;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 2;    /* indices 0 and 1 are unused */
    }

    /* Extend cache until it contains the desired index */
    while (cache_size <= target) {
        /* Bring in next row */
        ++last_row;

        /* Ensure we have all primes up to last_row */
        if (last_row > prime_limit) {
            uint64_t *new_primes = NULL;
            size_t   new_cnt = 0;
            build_primes_upto(last_row, &new_primes, &new_cnt);
            free(prime_arr);
            prime_arr  = new_primes;
            prime_cnt  = new_cnt;
            prime_limit = last_row;
        }

        /* Lower bound for primes in this row: floor(last_row/2)+1 */
        uint64_t lower = last_row / 2 + 1;
        uint64_t upper = last_row;

        /* Locate indices of first prime ≥ lower and first prime > upper */
        size_t start = prime_first_ge(prime_arr, prime_cnt, lower);
        size_t end   = prime_first_gt(prime_arr, prime_cnt, upper);

        /* Append primes of this row to the cache */
        for (size_t i = start; i < end; ++i) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)prime_arr[i];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* values below offset are unspecified */

    ensure_cache(n);
    return cache[n];
}
