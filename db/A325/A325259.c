/*
 * A325259  Numbers with one fewer distinct prime exponents than distinct
 *          prime factors.
 * https://oeis.org/A325259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325259 – Numbers whose prime factorisation has one fewer distinct
 * exponents than distinct prime factors.
 *
 * The function enumerates candidates in ascending order and keeps a
 * static cache of the values found.  For each new n it continues the
 * search from the last tested candidate.  This strategy works regardless
 * of whether calls to a(n) arrive in increasing order or not.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- static data for the dynamic cache ----------------------------------- */
static seq_t *cache = NULL;          /* array of computed a(k)               */
static size_t cache_len = 0;         /* number of elements currently stored  */
static size_t cache_cap = 0;         /* allocated capacity                   */

/* --- static data for prime generation ----------------------------------- */
static uint64_t *primes = NULL;      /* list of primes up to sqrt(candidate) */
static size_t primes_cnt = 0;        /* number of primes stored              */
static size_t primes_cap = 0;        /* allocated capacity for primes        */

/* --- static data for the scanning process ------------------------------- */
static uint64_t next_candidate = 2;  /* next integer to test                 */

/* --------------------------------------------------------------------------*/
/* Grow the prime buffer when it becomes full                          */
static void grow_primes(void)
{
    primes_cap = primes_cap ? primes_cap * 2 : 64;
    uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    primes = tmp;
}

/* Ensure that all primes up to sqrt(n) are present in the primes array.  */
static void ensure_primes_up_to_sqrt(uint64_t n)
{
    if (primes_cnt == 0) {
        if (primes_cap == 0) {
            primes_cap = 64;
            primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        }
        primes[0] = 2;
        primes_cnt = 1;
    }

    /* grow until the last prime squared is >= n */
    while ((uint64_t)primes[primes_cnt - 1] * primes[primes_cnt - 1] < n) {
        uint64_t next = primes[primes_cnt - 1] + 1;
        while (true) {
            bool is_prime = true;
            size_t i;
            for (i = 0; i < primes_cnt && primes[i] * primes[i] <= next; ++i) {
                if (next % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (primes_cnt >= primes_cap)
                    grow_primes();
                primes[primes_cnt++] = next;
                break;
            }
            ++next;
        }
    }
}

/* --------------------------------------------------------------*/
/* Test whether a number satisfies the sequence condition.
 * For a number m let k be the number of distinct prime factors
 * and d be the number of distinct exponents in its prime factorisation.
 * Return true iff d == k-1.
 */
static bool good_candidate(uint64_t m)
{
    uint64_t rem = m;
    int k = 0;                 /* distinct prime factors      */
    int distinct = 0;          /* distinct exponents         */
    unsigned char freq[128] = {0};   /* exponent frequency table */

    ensure_primes_up_to_sqrt(rem);

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            ++k;
            if (freq[e] == 0)
                ++distinct;
            ++freq[e];
        }
    }

    if (rem > 1) {   /* remaining prime factor (exponent 1) */
        ++k;
        int e = 1;
        if (freq[e] == 0)
            ++distinct;
        ++freq[e];
    }

    return distinct == k - 1;
}

/* --------------------------------------------------------------*/
/* Public sequence function */
seq_t a(idx_t n)
{
    /* n must be >= 1.  For n == 0 the sequence is undefined – return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Find the next number that satisfies the condition. */
        while (!good_candidate(next_candidate))
            ++next_candidate;

        /* Store the discovered value. */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            cache = tmp;
        }
        cache[cache_len++] = (seq_t)next_candidate;

        ++next_candidate;   /* next candidate to test on following call */
    }

    return cache[n - 1];
}
