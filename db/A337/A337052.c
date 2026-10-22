/*
 * A337052  Numbers k such that the powerful part of k has an even number of
 *          prime divisors counted with multiplicity.
 * https://oeis.org/A337052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the A337052 sequence:  
 * the numbers k such that the powerful part of k (product of prime powers
 * with exponent ≥ 2) has an even total number of prime factors counted
 * with multiplicity.  The value is obtained by an incremental sieve and
 * a cache to allow random access. */
#include <stdlib.h>

/*--- Sieve of smallest prime factors ---------------------------------*/
static idx_t *spf = NULL;          /* lowest prime factor for each i   */
static idx_t  spf_limit = 1;       /* maximum index spf covers */

/* Compute spf up to `limit` (inclusive). */
static void compute_spf(idx_t limit)
{
    /* Allocate new SPF array (must cast in C++). */
    idx_t *new_spf = (idx_t *)calloc((size_t)limit + 1, sizeof(idx_t));
    if (!new_spf) abort();
    new_spf[1] = 1;

    /* Linear sieve: store primes in a dynamic array. */
    idx_t *primes = NULL;
    size_t primes_cap = 0, primes_sz = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (primes_sz == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 10;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) { free(new_spf); abort(); }
            }
            primes[primes_sz++] = i;
        }
        for (size_t j = 0; j < primes_sz &&
             primes[j] <= new_spf[i] &&
             i * primes[j] <= limit; ++j)
        {
            new_spf[i * primes[j]] = primes[j];
        }
    }
    free(primes);

    /* Replace old table. */
    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/*--- Sequence cache ---------------------------------------------------*/
static seq_t *cache = NULL;      /* array of cached terms          */
static size_t cache_len = 0;     /* number of cached terms          */
static size_t cache_cap = 0;     /* allocated capacity in cache     */
static idx_t  cur_k    = 1;      /* next integer candidate to test */

/* Return the n‑th term of the sequence (1‑based).  The function
 * expands the cache as needed and stores its values for subsequent calls. */
seq_t a(idx_t n)
{
    /* Initialize cache lazily. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until the cache contains at least n entries. */
    while (cache_len < (size_t)n) {
        /* Ensure the SPF table is large enough. */
        if (cur_k > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit * 2 : 2;
            while (new_limit < cur_k) new_limit *= 2;
            compute_spf(new_limit);
        }

        /* Test cur_k for the parity condition. */
        idx_t temp  = cur_k;
        int   parity = 0;   /* 0 = even, 1 = odd */

        while (temp > 1) {
            idx_t p = spf[temp];
            int   e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e >= 2)
                parity ^= (e & 1);
        }

        if (parity == 0) {                /* include if powerful part has even parity */
            if (cache_len + 1 > cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len] = (seq_t)cur_k;
            ++cache_len;
        }

        ++cur_k;
    }

    return cache[n - 1];
}
