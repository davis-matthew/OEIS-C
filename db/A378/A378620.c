/*
 * A378620  Lesser prime index of twin primes with nonsquarefree mean.
 * https://oeis.org/A378620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 616, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the lesser prime index of twin primes whose mean (p+1) is not squarefree.
   The function grows a sieve of primes and a cache of valid indices until the
   requested term is available.  The algorithm is purely constructive
   and does not use any hard‑coded values. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

static uint64_t *primes = NULL;      /* current list of primes */
static size_t   primes_cnt = 0;      /* number of primes in the list   */
static idx_t    prime_limit = 0;     /* current sieve bound             */
static idx_t    processed_idx = 0;   /* next prime index to process     */

static seq_t  *terms = NULL;         /* cached sequence values          */
static size_t  terms_cnt = 0;        /* number of terms cached          */
static size_t  terms_cap = 0;        /* capacity of the cache           */

/* Allocate or grow the terms array to hold at least 'needed' entries. */
static void grow_terms(size_t needed)
{
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Allocate or extend the sieve to include all primes up to 'limit'. */
static void build_sieve(idx_t limit)
{
    free(primes);                       /* reclaim previous array (freeing NULL is safe) */
    build_primes_upto(limit, &primes, &primes_cnt);
    prime_limit = limit;
}

/* Ensure that the sequence has at least 'target' terms. */
static void ensure_terms(idx_t target)
{
    if (target == 0) return;                  /* nothing to compute for n = 0 */

    /* Initial build of a reasonably large sieve if none yet. */
    if (prime_limit == 0) {
        build_sieve(2000000ULL);               /* ~2 M enough for n≈10 000 */
    }

    grow_terms(target);

    while (terms_cnt < target) {
        /* If we have reached the end of the current prime list,
           extend the sieve to the next bound. */
        if (processed_idx + 1 >= primes_cnt) {
            idx_t new_limit = prime_limit * 2;
            if (new_limit < prime_limit) new_limit = UINT64_MAX; /* overflow guard */
            build_sieve(new_limit);
        }

        /* Examine the next consecutive prime pair. */
        uint64_t p = primes[processed_idx];
        uint64_t q = primes[processed_idx + 1];

        if (q == p + 2) {                     /* twin prime found          */
            seq_t mean = (seq_t)p + (seq_t)1;  /* mean = p + 1              */
            if (!is_squarefree(mean)) {       /* mean not squarefree       */
                /* Append the 1‑based index of 'p': */
                terms[terms_cnt++] = (seq_t)(processed_idx + (seq_t)1);
            }
        }

        processed_idx++;
    }
}

/* Public function required by the exerciser. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0, harmless */
    ensure_terms(n);
    return terms[n - 1];
}
