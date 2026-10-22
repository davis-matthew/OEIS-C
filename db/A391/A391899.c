/*
 * A391899  The maximum exponent in the prime factorization of the
 *          exponentially squarefree numbers (A209061).
 * https://oeis.org/A391899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A051903(A209061(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the maximum exponent in the prime factorization of the n-th
 * exponentially squarefree number (OEIS A209061).
 * The function computes the sequence by generating exponentially
 * squarefree numbers in ascending order using a SPF sieve.
 * The first term a(1) is 0 (for number 1).
 * Subsequent terms are obtained by iterating over integers and
 * checking that every prime exponent in their factorization is
 * squarefree; the largest such exponent is returned.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_squarefree_exponent.h"

/* forward declarations */
static void init_spf(size_t limit);
static bool is_squarefree_exponent(uint32_t e);
static bool esf_and_max_exp(uint64_t x, seq_t *maxe);
static const size_t INITIAL_SPF_LIMIT = 1000000;   /* initial sieve size */

/* static state kept across calls */
static uint32_t *spf = NULL;           /* smallest prime factor array */
static size_t spf_limit = 0;           /* current size of spf array   */
static seq_t  *cache = NULL;           /* cached a(n) values          */
static size_t  cache_size = 0;         /* current number of cached terms */
static size_t  cache_cap  = 0;         /* capacity of cache array      */
static uint64_t candidate  = 2;        /* next integer to test         */

/* --------------------------------------------------------------- */
/* Initialise the SPF sieve up to 'limit' (inclusive).            */
/* --------------------------------------------------------------- */
static void init_spf(size_t limit)
{
    /* free old array if any */
    free(spf);
    spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);          /* abort on allocation failure */
    memset(spf, 0, (limit + 1) * sizeof(uint32_t));

    /* simple linear sieve for SPF */
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            /* avoid overflow when computing i*i */
            if (i <= (size_t)((double)limit / i)) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (!spf[j]) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* --------------------------------------------------------------- */
/* Check whether an exponent e (trial division) is squarefree.     */

/* --------------------------------------------------------------- */
/* Test whether x is exponentially squarefree and, if so, compute
 * its maximum prime exponent.  Return true on success.
 * --------------------------------------------------------------- */
static bool esf_and_max_exp(uint64_t x, seq_t *maxe)
{
    if (x == 1) { *maxe = (seq_t)0; return true; }

    uint64_t temp = x;
    seq_t max_exp = 0;

    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t e  = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        if (!is_squarefree_exponent(e)) return false;
        if ((seq_t)e > max_exp) max_exp = (seq_t)e;
    }
    *maxe = max_exp;
    return true;
}

/* --------------------------------------------------------------- */
/* Main sequence function.                                        */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* first call: initialise static state */
    if (cache_size == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;   /* a(1) = 0 */
        cache_size = 1;
        candidate = 2;         /* start checking from 2 */
        init_spf(INITIAL_SPF_LIMIT);
    }

    /* extend cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* Ensure sieve covers the candidate number */
        if (candidate > spf_limit) {
            size_t new_limit = spf_limit * 2;
            if (new_limit < candidate) new_limit = candidate;
            init_spf(new_limit);
        }

        /* Check candidate and record max exponent if ESF */
        seq_t max_exp;
        if (esf_and_max_exp(candidate, &max_exp)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_size++] = max_exp;
        }
        ++candidate;
    }

    return cache[n - 1];
}
