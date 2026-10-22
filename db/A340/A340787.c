/*
 * A340787  Heinz numbers of integer partitions of positive rank.
 * https://oeis.org/A340787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A340787.
   The sequence consists of the Heinz numbers of all integer partitions
   that have a positive Dyson rank.
   We generate integers in increasing order, factor them with a small‑prime‑factor sieve
   to obtain the partition parts, compute the rank, and keep those with
   a positive rank. */

/*--- Includes ----------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- Static tables ------------------------------------------------------*/
static seq_t   *cache     = NULL;   /* cached sequence values   */
static size_t  cache_cap  = 0;      /* allocated capacity       */
static size_t  cache_len  = 0;      /* number of values stored  */

static uint64_t *spf      = NULL;  /* smallest prime factor array  */
static uint32_t *prime_idx= NULL;  /* index (1‑based) of every prime */
static idx_t    sieve_limit = 0;   /* current maximum number sieved */

static idx_t    last_checked = 1;  /* largest m already examined */

/*--- Helper prototypes -----------------------------------------------*/
static void extend_sieve(idx_t limit);
static void grow_cache(idx_t need);
static bool positive_rank(idx_t m);

/*--- Extend the sieve up to at least 'limit' ---------------------------*/
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate fresh tables */
    uint64_t *new_spf  = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    uint32_t *new_idx  = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!new_spf || !new_idx) exit(1);   /* out of memory */

    uint32_t prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {             /* i is prime */
            new_spf[i]   = i;
            new_idx[i]   = ++prime_count;   /* record its ordinal index */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    /* Replace old tables */
    if (spf)      free(spf);
    if (prime_idx) free(prime_idx);
    spf        = new_spf;
    prime_idx  = new_idx;
    sieve_limit = limit;
}

/*--- Ensure the cache can hold at least 'need' elements --------------*/
static void grow_cache(idx_t need)
{
    if (cache_cap >= (size_t)need) return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/*--- Test whether an integer m has a positive Dyson rank -------*/
static bool positive_rank(idx_t m)
{
    int parts = 0;
    int maxpart = 0;
    while (m > 1) {
        uint64_t p = spf[m];
        int  idx   = prime_idx[p];
        if (idx > maxpart) maxpart = idx;
        /* Count multiplicity */
        do {
            ++parts;
            m /= p;
        } while (m > 1 && spf[m] == p);
    }
    return maxpart > parts;
}

/*--- Main sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* Initialise sieve on first call. */
    if (!spf) extend_sieve(16);

    /* Make sure we have at least n values cached. */
    if (cache_len < n) {
        grow_cache(n);
        for (idx_t m = last_checked + 1; cache_len < n; ++m) {
            /* Extend sieve if we have reached the current limit. */
            if (m > sieve_limit) {
                idx_t new_limit = sieve_limit ? sieve_limit << 1 : 64;
                if (new_limit < m) new_limit = m;
                extend_sieve(new_limit);
            }

            if (positive_rank(m))
                cache[cache_len++] = (seq_t)m;
            last_checked = m;
        }
    }
    return cache[n - 1];
}
