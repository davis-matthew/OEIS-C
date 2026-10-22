/*
 * A336353  Numbers k such that sigma(k) does not have any prime factor larger
 *          than the largest prime factor of k.
 * https://oeis.org/A336353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A336353
    Numbers k such that sigma(k) does not have any prime factor larger than the
    largest prime factor of k.
    Implements a search/sieve method to produce the sequence in ascending
    order.  The result is returned for any n >= 1 in any order of calls
    (memoised through static state).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper data structures ---------- */
static uint64_t *spf = NULL;                /* smallest prime factor for each n */
static uint64_t *primes = NULL;             /* list of primes */
static size_t primes_cnt = 0;               /* number of primes generated */
static idx_t sieve_limit = 0;               /* highest number for which spf & primes are valid */

static seq_t *cache = NULL;                /* cached sequence values */
static size_t cache_len = 0;                /* number of values cached */
static size_t cache_cap = 0;                /* capacity of cache array */
static idx_t next_k = 2;                    /* next candidate k to examine */

/* ---------- utility functions ---------- */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    free(spf);
    free(primes);

    /* Allocate spf array (+1 for index 0) and primes array */
    spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (spf == NULL) abort();
    primes = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (primes == NULL) abort();

    size_t pcount = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pcount++] = (uint64_t)i;
        }
        for (size_t j = 0; j < pcount; ++j) {
            uint64_t p = primes[j];
            idx_t ip = i * p;
            if (ip > limit) break;
            spf[ip] = p;
            if (p == spf[i]) break;
        }
    }
    primes_cnt = pcount;
    sieve_limit = limit;
}

static seq_t largest_pf_of_k(idx_t k)
{
    if (k <= 1) return 1;
    idx_t t = k;
    seq_t maxp = 1;
    while (t > 1) {
        uint64_t p = spf[t];
        if (p > maxp) maxp = (seq_t)p;
        while (t % p == 0) t /= p;
    }
    return maxp;
}

static seq_t sigma_of_k(idx_t k)
{
    if (k <= 1) return 1;
    idx_t t = k;
    seq_t result = 1;
    while (t > 1) {
        uint64_t p = spf[t];
        size_t exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        /* compute 1 + p + p^2 + ... + p^exp */
        seq_t term = 1;
        seq_t curr = 1;
        for (size_t i = 0; i < exp; ++i) {
            curr *= (seq_t)p;
            term += curr;
        }
        result *= term;
    }
    return result;
}

static seq_t largest_pf_of_sigma(seq_t sigma)
{
    seq_t t = sigma;
    seq_t maxp = 1;
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        seq_t p = (seq_t)primes[idx];
        seq_t p2 = p * p;
        if (p2 > t) break;
        if (t % p == 0) {
            maxp = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) maxp = t;            /* remaining prime factor */
    return maxp;
}

static void append_value(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_arr == NULL) abort();
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n=0, return 0 just in case */

    if (cache_len == 0) {        /* initialise on first call */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        append_value(1);         /* a(1) = 1 */
    }

    /* ensure initial sieve large enough; start with 1e5 */
    if (sieve_limit == 0) ensure_sieve(100000);

    /* extend sieve if needed for upcoming candidates */
    while (cache_len < n) {
        /* extend sieve when next_k becomes large relative to current limit */
        if ((idx_t)(2 * next_k) > sieve_limit)
            ensure_sieve(sieve_limit * 2);

        seq_t max_pf_k = largest_pf_of_k(next_k);
        seq_t sigma_k = sigma_of_k(next_k);
        seq_t max_pf_sigma = largest_pf_of_sigma(sigma_k);

        if (max_pf_sigma <= max_pf_k)
            append_value((seq_t)next_k);

        ++next_k;
    }

    return cache[n - 1];
}
