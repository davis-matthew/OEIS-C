/*
 * A376250  Numbers with a unique largest prime exponent (A356862) that are
 *          not prime powers (A246655).
 * https://oeis.org/A376250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A376250:
 *  Numbers whose unordered prime signature contains a unique largest exponent
 *  and at least two distinct prime factors (i.e. not a prime power).
 *  a(1) = 12, a(2) = 18, …
 *
 *  The implementation builds the sequence on demand by scanning upward
 *  from the last generated value, factoring with a small prime sieve,
 *  and storing the results in a static cache.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Prime sieve (up to a fixed upper bound)                               */
static seq_t *primes = NULL;          /* array of primes (seq_t type)   */
static size_t primes_cap = 0;         /* capacity of primes array      */
static size_t primes_cnt = 0;         /* number of primes stored       */
static bool primes_initialized = false;

/* Initialize the prime list up to LIMIT (source: simple Sieve of Eratosthenes). */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    primes_initialized = true;

    const uint64_t LIMIT = 200000;  /* covers required sqrt range for n ≤ 10000 */
    bool *comp = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!comp)
        return;  /* allocation failed – fallback will happen but unlikely here */

    primes_cap = 25000;           /* enough for 2e5 (pi(200k) < 17984)            */
    primes = (seq_t *)calloc(primes_cap, sizeof(seq_t));
    if (!primes) {
        free(comp);
        return;
    }

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            if (primes_cnt == primes_cap) {
                /* grow array if necessary (unlikely for this size) */
                primes_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt++] = (seq_t)i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
}

/* --------------------------------------------------------------------- */
/* Cache of sequence terms                                               */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t  last_checked = 1;  /* last integer examined during generation */

/* Append a new term to the cache */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return;  /* allocation failed – cannot store more terms */
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Test whether x satisfies the sequence condition                       */
static bool good_number(seq_t x)
{
    /* Factor x using precomputed primes */
    int exps[64];          /* exponents of distinct prime factors */
    int exps_cnt = 0;

    seq_t n = x;
    init_primes();        /* ensure primes are ready */

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        seq_t p = primes[pi];
        if (p > 0 && n / p < p)   /* p*p > n; stop when p > sqrt(n) */
            break;

        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        if (e > 0) {
            if (exps_cnt == (int)(sizeof(exps)/sizeof(exps[0])))
                break;  /* too many prime factors – unlikely here */
            exps[exps_cnt++] = e;
        }
    }
    if (n > 1) {   /* remaining prime factor */
        if (exps_cnt == (int)(sizeof(exps)/sizeof(exps[0])))
            return false;
        exps[exps_cnt++] = 1;
    }

    if (exps_cnt < 2)
        return false;             /* need at least two distinct primes */

    /* Find maximum exponent and its frequency */
    int maxe = 0, freq = 0;
    for (int i = 0; i < exps_cnt; ++i) {
        if (exps[i] > maxe) {
            maxe = exps[i];
            freq = 1;
        } else if (exps[i] == maxe) {
            ++freq;
        }
    }
    return freq == 1;              /* unique largest exponent */
}

/* --------------------------------------------------------------------- */
/* Public function requested by the framework                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, but return 0 safely */

    size_t target = (size_t)n;      /* n fits into size_t for n ≤ 10000 */

    /* Generate terms until we have n of them */
    while (cache_len < target) {
        last_checked = last_checked + 1;
        if (good_number(last_checked))
            add_term(last_checked);
    }
    return cache[n - 1];
}
