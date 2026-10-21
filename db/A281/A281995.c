/*
 * A281995  Squarefree numbers that, when added to the sum of their prime
 *          factors, remain squarefree.
 * https://oeis.org/A281995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth squarefree integer n such that n +
 *            sum_of_distinct_prime_factors(n) is also squarefree
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/floor_isqrt_u64.h"

/* ------------------------------------------------------------------
   Prime sieve utilities
------------------------------------------------------------------ */

static uint32_t *prime_list = NULL;  /* array of primes            */
static size_t   prime_cnt  = 0;      /* number of primes           */
static size_t   prime_cap  = 0;      /* allocated capacity         */
static uint32_t prime_limit = 0;     /* largest prime considered   */

/* Sieve of Eratosthenes up to 'limit' (inclusive) and store primes. */
static void generate_primes_up_to(uint32_t limit)
{
    bool *comp = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    memset(comp, 0, (size_t)(limit + 1) * sizeof(bool));

    for (uint32_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                comp[j] = true;

    /* replace old list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_cap  = limit / 10 + 10;
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t*)realloc(prime_list,
                                               prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = i;
        }
    }
    free(comp);
    prime_limit = limit;
}

/* Ensure that the prime list contains all primes up to at least
   sqrt(v)+1, extending it if necessary. */
static void ensure_primes_upto(uint64_t v)
{
    uint32_t need = (uint32_t)(floor_isqrt_u64(v) + 1);
    if (prime_limit < need)
        generate_primes_up_to(need * 2 + 100);   /* extra margin */
}

/* ------------------------------------------------------------------
   Squarefree and sum-of-primes test
------------------------------------------------------------------ */

/* Return true if x is squarefree and x + sum_of_distinct_prime_factors(x)
   is also squarefree. */
static bool satisfies_condition(uint64_t x)
{
    if (x == 1)          /* 1 is squarefree and sum = 0 */
        return true;

    ensure_primes_upto(x);
    uint64_t orig = x;
    uint64_t sum_pf = 0;

    /* factor x */
    uint64_t temp = x;
    for (size_t i = 0; i < prime_cnt &&
                     (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp;
         ++i)
    {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            unsigned cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1)        /* repeated prime factor */
                    return false;
            }
            sum_pf += p;
        }
    }
    if (temp > 1)  /* remaining prime factor > sqrt(orig) */
        sum_pf += temp;

    uint64_t target = orig + sum_pf;
    ensure_primes_upto(target);

    /* test target for squarefreeness */
    temp = target;
    for (size_t i = 0; i < prime_cnt &&
                     (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp;
         ++i)
    {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            unsigned cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1)        /* repeated prime factor */
                    return false;
            }
        }
    }
    /* temp > 1 means a single prime factor remains – already squarefree */
    return true;
}

/* ------------------------------------------------------------------
   Cache of computed terms
------------------------------------------------------------------ */

static seq_t *term_cache     = NULL;   /* stored sequence values   */
static size_t term_cap       = 0;      /* array capacity           */
static size_t term_cnt       = 0;      /* number of terms cached   */
static uint64_t next_candidate = 1;    /* next integer to test     */

/* Append a new term to the cache, expanding if needed. */
static void append_term(seq_t val)
{
    if (term_cnt == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 8;
        term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
    }
    term_cache[term_cnt++] = val;
}

/* ------------------------------------------------------------------
   Public function
------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Lazily generate terms until at least n terms are cached. */
    while (term_cnt < n) {
        uint64_t cand = next_candidate++;
        if (satisfies_condition(cand))
            append_term((seq_t)cand);
    }
    return term_cache[n - 1];
}
