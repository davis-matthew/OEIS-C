/*
 * A307516  Numbers whose maximum prime index and minimum prime index differ
 *          by more than 1.
 * https://oeis.org/A307516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes A307516 by enumerating integers and selecting those
 *            whose max prime index minus min prime index > 1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

/* -------------  static data  --------------------------------- */

static idx_t *spf = NULL;                /* smallest prime factor array */
static uint64_t *primes = NULL;          /* list of primes (up to current sieve limit) */
static size_t prime_cnt = 0;             /* number of primes in primes[] */

/* mapping from prime value to its 1‑based index (prime_index_arr[p] = index);
   a zero value means p is composite or outside the sieve */
static idx_t *prime_index_arr = NULL;
static size_t prime_index_size = 0;      /* allocated size of prime_index_arr */

static idx_t sieve_limit = 0;            /* maximum sieve size built so far */

static seq_t *term_cache = NULL;         /* cached terms a(1), a(2), ...  */
static size_t cache_len = 0;             /* number of terms currently cached   */
static size_t cache_cap = 0;             /* allocated capacity of term_cache   */

static idx_t candidate = 2;              /* next integer to be tested          */

/* -------------  helper functions  ----------------------------- */

/* Expand the sieve to cover at least 'need'.  Rebuilds spf, primes and the
   prime_index_arr array.  The sieve size is doubled each time to keep
   amortized cost low. */
static void
ensure_sieve(idx_t need)
{
    if (need <= sieve_limit) return;

    idx_t new_limit = sieve_limit ? sieve_limit : 1000;
    while (new_limit < need) new_limit <<= 1;

    /* free old data */
    free(spf);
    free(primes);
    free(prime_index_arr);

    /* build new sieve */
    build_spf(new_limit, &spf);
    build_primes_upto(new_limit, &primes, &prime_cnt);

    /* allocate prime_index_arr */
    prime_index_arr = (idx_t *)calloc((size_t)new_limit + 1, sizeof(idx_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_index_arr[primes[i]] = (idx_t)(i + 1);   /* 1‑based index */

    sieve_limit   = new_limit;
    prime_index_size = (size_t)new_limit + 1;
}

/* Ensure the term cache has capacity for 'must' terms.  This is an O(1)
   amortised operation. */
static void
ensure_cache_capacity(size_t must)
{
    if (cache_cap >= must) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < must) new_cap <<= 1;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Test whether the integer 'x' has a maximum prime index minus minimum
   prime index exceeding 1.  The factorisation uses the precomputed SPF
   array. */
static bool
good_number(idx_t x)
{
    idx_t min_idx = 0, max_idx = 0;
    while (x > 1) {
        idx_t p = spf[x];
        idx_t idx = prime_index_arr[p];    /* 1‑based index of p */
        if (min_idx == 0) {                /* first prime factor */
            min_idx = max_idx = idx;
        } else {
            if (idx < min_idx) min_idx = idx;
            if (idx > max_idx) max_idx = idx;
            if (max_idx - min_idx > 1) return true;   /* early exit */
        }
        while (x % p == 0) x /= p;
    }
    return (max_idx - min_idx > 1);
}

/* -------------  main sequence function  ----------------------- */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller */
    while (cache_len < n) {
        ensure_sieve(candidate);
        if (good_number(candidate)) {
            ensure_cache_capacity(cache_len + 1);
            term_cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return term_cache[n - 1];
}
