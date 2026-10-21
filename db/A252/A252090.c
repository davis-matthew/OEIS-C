/*
 * A252090  Primes p such that p + 28 is also prime.
 * https://oeis.org/A252090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that p+28 is also prime.
   The algorithm sieves a growing limit until it has found enough
   terms.  It uses the supplied build_primes_upto and isPrimeGeneric
   helpers. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeGeneric.h"

static idx_t sieve_limit = 0;           /* current upper bound of sieve */
static uint64_t *prime_list = NULL;     /* primes up to sieve_limit          */
static size_t prime_cnt = 0;            /* number of primes   */
static seq_t *pair_list = NULL;        /* primes p with p+28 prime          */
static size_t pair_cnt = 0;             /* number of such primes */

/* Rebuild the prime list and the pair list for a given limit. */
static void recompute_pairs(idx_t new_limit)
{
    /* Free old data, if any. */
    free(prime_list);
    free(pair_list);

    /* Build list of all primes up to new_limit. */
    uint64_t *pr;   size_t cnt;
    build_primes_upto(new_limit, &pr, &cnt);
    prime_list = pr;
    prime_cnt  = cnt;

    /* Count how many pairs exist up to new_limit. */
    size_t temp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if ((idx_t)(p + 28) <= new_limit && isPrimeGeneric(p + 28))
            ++temp;
    }

    /* Allocate and fill the pair list. */
    pair_list = (seq_t *)malloc(temp * sizeof(seq_t));
    size_t j = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p + 28 <= new_limit && isPrimeGeneric((seq_t)(p + 28))) {
            pair_list[j++] = (seq_t)p;
        }
    }
    pair_cnt = temp;
}

/* Ensure that pair_list contains at least n terms by growing the sieve
   limit until enough pairs have been found. */
static void ensure_pairs(idx_t n)
{
    if (pair_cnt >= n) return;

    /* Start with a reasonable initial limit. */
    if (sieve_limit == 0) sieve_limit = 200000;  /* covers first 10000 terms */

    while (pair_cnt < n) {
        /* Double the limit (with a small positive offset) each time. */
        sieve_limit = sieve_limit * 2 + 1000;
        recompute_pairs(sieve_limit);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_pairs(n);
    return pair_list[n - 1];
}
