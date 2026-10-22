/*
 * A345452  Positive integers with an even number of prime factors (counting
 *          repetitions) that sum to an even number.
 * https://oeis.org/A345452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The sequence A345452 consists of all positive integers n such that
 * the total number of prime factors of n (counting multiplicity) is even
 * and the sum of those prime factors is even.
 * 
 * The property can be tested by factoring n and counting:
 *   c = Σ e_p   (total number of prime factors)
 *   s = Σ e_p * p   (sum of prime factors)
 *   n ∈ A if (c mod 2 == 0) && (s mod 2 == 0).
 *
 * The implementation below generates the terms in order by
 * scanning successive integers and appending those that satisfy
 * the test.  A dynamic SPF table (smallest prime factor) is used
 * to speed up factorisation, and the resulting values are cached
 * so that arbitrary calling order of a(idx_t) is supported.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ----- static data structures ----- */
static uint32_t *spf  = NULL;      /* smallest prime factor table (0..N)    */
static size_t   spf_limit = 0;     /* largest index for which spf is valid */

static uint32_t *primes    = NULL; /* list of primes for linear sieve      */
static size_t   primes_cnt = 0;    /* number of primes stored              */

static seq_t  *cache  = NULL;      /* cached sequence values               */
static size_t  cache_size = 0;     /* number of values stored              */
static size_t  cache_cap  = 0;     /* allocated capacity                    */
static uint64_t current_check = 1; /* next integer to test                 */

/* ------------------------------------------------------------------ */
/* Extend the SPF table and prime list up to at least 'limit'.       */
/* ------------------------------------------------------------------ */
static void ensure_spf(uint64_t limit)
{
    if (limit <= spf_limit)
        return;

    /* Grow SPF array.  */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf)
        abort();
    spf = new_spf;

    /* Initialise the new part to 0.  */
    for (uint64_t i = spf_limit + 1; i <= limit; ++i)
        spf[i] = 0;

    /* Linear sieve extending the existing prime list.  */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            /* Grow prime list.  */
            uint32_t *tmp = (uint32_t *)realloc(primes, (primes_cnt + 1) * sizeof(uint32_t));
            if (!tmp)
                abort();
            primes = tmp;
            primes[primes_cnt++] = (uint32_t)i;
        }

        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            uint64_t v = i * p;
            if (v > limit)
                break;
            spf[v] = (uint32_t)p;
            if (p == spf[i])
                break;
        }
    }
    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Test whether integer 'x' satisfies the sequence condition.        */
/* ------------------------------------------------------------------ */
static bool satisfies(uint64_t x)
{
    unsigned int count = 0;
    unsigned long long sum = 0;

    uint64_t tmp = x;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        ++count;
        sum += p;
        tmp /= p;
    }
    return ((count & 1) == 0) && ((sum & 1) == 0);
}

/* ------------------------------------------------------------------ */
/* Extend the cache until it contains at least 'n' terms.             */
/* ------------------------------------------------------------------ */
static void ensure_cache(size_t n)
{
    if (cache_size >= n)
        return;

    /* Grow cache capacity if necessary.  */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    } else {
        while (cache_cap < n)
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Scan natural numbers, appending those that satisfy the test.  */
    while (cache_size < n) {
        uint64_t m = current_check++;
        if (m < 2) {               /* 1 always satisfies (no prime factors)   */
            if (cache_size < n)
                cache[cache_size++] = (seq_t)m;
            continue;
        }

        /* Ensure SPF table can factorize 'm'.  */
        ensure_spf(m);

        if (satisfies(m))
            cache[cache_size++] = (seq_t)m;
    }
}

/* ------------------------------------------------------------------ */
/* Public function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for zero, return 0 as a fallback */

    ensure_cache((size_t)n);
    return cache[n - 1];
}
