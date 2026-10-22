/*
 * A353839  Numbers whose prime indices do not have all distinct run-sums.
 * https://oeis.org/A353839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   duplicate_run_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A353839: Numbers whose prime indices do not have all distinct
 * run-sums.
 *
 * This implementation builds the sequence on demand, caching all
 * terms that have been found so far.  It uses a simple linear sieve
 * to factor numbers up to the current search limit.  Every term is
 * stored in a dynamically sized array and is returned when the
 * requested index has been reached.
 *
 * The algorithm runs in O(n log n) time for the first n terms,
 * easily satisfying the requirement for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/duplicate_run_sums.h"

/*===========================================================================*/
/*  Cached data and helper functions                                      */
/*===========================================================================*/

static seq_t *terms      = NULL;     /* cached sequence values           */
static size_t terms_cap  = 0;        /* array capacity                   */
static size_t terms_len  = 0;        /* number of computed terms         */

static idx_t  search_start = 2;      /* next integer to test             */

static uint32_t *spf            = NULL;   /* smallest prime factor array      */
static uint32_t *prime_index    = NULL;   /* 1‑based index of the prime       */
static uint32_t *primes         = NULL;   /* list of primes for the sieve     */
static size_t   primes_cap      = 0;      /* primes array capacity            */
static size_t   primes_cnt      = 0;      /* number of primes found           */
static idx_t    sieve_limit     = 0;      /* current maximum sieved value     */

/*--------------------------------------------------------------------------*/
/*  Ensure that spf, prime_index and primes contain all numbers up to ~n   */
/*--------------------------------------------------------------------------*/
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;                       /* already available */

    /* Increase the limit a bit to avoid reallocating on every step.     */
    idx_t limit = n + 1000;
    free(spf);
    free(prime_index);
    free(primes);

    spf          = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    prime_index  = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    primes_cap   = (size_t)limit / 10 + 1000;
    primes       = (uint32_t *)calloc(primes_cap, sizeof(uint32_t));

    size_t pcnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!spf[i]) {                       /* i is prime            */
            spf[i]          = (uint32_t)i;
            primes[pcnt]    = (uint32_t)i;
            prime_index[i]  = (uint32_t)(pcnt + 1); /* 1‑based index */
            ++pcnt;
        }
        for (size_t j = 0; j < pcnt &&
             primes[j] <= spf[i] &&
             i * primes[j] <= (size_t)limit; ++j)
        {
            spf[i * primes[j]] = primes[j];
        }
    }
    primes_cnt  = pcnt;
    sieve_limit = limit;
}

/*--------------------------------------------------------------------------*/
/*  Append a new term to the cache.  Grow the array when needed.            */
/*--------------------------------------------------------------------------*/
static void append_term(seq_t value)
{
    if (terms_len == terms_cap) {
        size_t newcap = terms_cap ? terms_cap << 1 : 32;
        seq_t *newarr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        if (!newarr) abort();               /* out of memory       */
        terms   = newarr;
        terms_cap = newcap;
    }
    terms[terms_len++] = value;
}

/*--------------------------------------------------------------------------*/
/*  Detect if a run‑sums multiset contains any duplicate values.           */

/*===========================================================================*/
/*  The actual OEIS sequence function                                       */
/*===========================================================================*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for 0, but harmless */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Make sure we can factor the current candidate.        */
        ensure_sieve(search_start);

        idx_t m = search_start;
        if (m < 2) {                 /* skip 0 and 1 */
            ++search_start;
            continue;
        }

        /* Factor m using the precomputed spf array. */
        idx_t temp = m;
        const size_t MAX_RUNS = 64;          /* generous upper bound    */
        seq_t run_sums[MAX_RUNS];
        size_t run_cnt = 0;

        uint32_t prev_idx = 0;
        bool     prev_set = false;
        uint32_t run_len  = 0;

        while (temp > 1) {
            uint32_t p = spf[(size_t)temp];
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint32_t idx = prime_index[p];  /* 1‑based index of p          */

            if (!prev_set) {               /* first prime of this number   */
                prev_idx = idx;
                run_len  = exp;
                prev_set = true;
            } else if (idx == prev_idx) {   /* same index → extend run     */
                run_len += exp;
            } else {                        /* new index → close previous run */
                run_sums[run_cnt++] = (seq_t)prev_idx * (seq_t)run_len;
                prev_idx = idx;
                run_len  = exp;
            }
        }
        if (prev_set) {
            run_sums[run_cnt++] = (seq_t)prev_idx * (seq_t)run_len;
        }

        /* Store m if the run sums are not all distinct. */
        if (duplicate_run_sums(run_sums, run_cnt))
            append_term((seq_t)m);

        ++search_start;
    }

    return terms[n - 1];
}
