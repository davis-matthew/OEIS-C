/*
 * A317240  Number of representations of n of the form 1 + p1 * (1 + p2* ... *
 *          (1 + p_j)...), where [p1, ..., p_j] is a (possibly empty) list of
 *          (not necessarily distinct) primes.
 * https://oeis.org/A317240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9987, u16 n <= 9987, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317240:  Number of representations of n as
   1 + p1*(1 + p2*( … *(1 + pj) …) ), where the pi are primes.
   Recurrence:
   a(1) = 1
   a(n) = Σ_{p | (n-1)} a((n-1)/p)   for n > 1
   (sum over distinct prime divisors of (n-1))
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------------------------------------------------------- */
/* static storage for the computed sequence and primes       */
/* ---------------------------------------------------------- */
static seq_t *cache = NULL;   /* 1‑based: cache[n] = a(n)            */
static size_t cache_cap = 0;   /* highest n computed                   */

static uint64_t *primes_arr   = NULL;  /* primes up to prime_limit          */
static size_t   primes_cap   = 0;       /* allocated capacity                 */
static size_t   primes_cnt   = 0;       /* number of primes stored            */
static idx_t    prime_limit  = 0;       /* largest checked for primality      */

/* ---------------------------------------------------------- */
/* simple integer square‑root: floor(sqrt(n))                 */
/* ---------------------------------------------------------- */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* ---------------------------------------------------------- */
/* ensure that all primes <= limit are present in primes_arr*/
/* ---------------------------------------------------------- */
static void ensure_primes_upto(idx_t limit)
{
    if (limit < 2) return;            /* nothing to add */
    if (limit <= prime_limit) return; /* already up to date */

    /* start from the next candidate */
    idx_t candidate = prime_limit + 1;
    if (candidate < 2) candidate = 2;

    /* grow storage if needed */
    if (primes_cap < primes_cnt + 1) {
        size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
        uint64_t *new_arr = (uint64_t *)realloc(primes_arr, new_cap * sizeof(uint64_t));
        if (!new_arr && new_cap > 0) exit(EXIT_FAILURE);
        primes_arr = new_arr;
        primes_cap = new_cap;
    }

    /* trial division extension */
    for (idx_t x = candidate; x <= limit; ++x) {
        bool is_prime = true;
        idx_t sq = isqrt(x);
        for (size_t j = 0; j < primes_cnt && primes_arr[j] <= sq; ++j) {
            if (x % primes_arr[j] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint64_t *new_arr = (uint64_t *)realloc(primes_arr,
                                            new_cap * sizeof(uint64_t));
                if (!new_arr && new_cap > 0) exit(EXIT_FAILURE);
                primes_arr = new_arr;
                primes_cap = new_cap;
            }
            primes_arr[primes_cnt++] = (uint64_t)x;
        }
    }
    prime_limit = limit;
}

/* ---------------------------------------------------------- */
/* ensure that cache holds values up to index n               */
/* ---------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (!cache) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));  /* indices 0,1 */
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cache_cap = 1;
    }
    if (n <= cache_cap) return;

    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        if (i == 1) { cache[i] = (seq_t)1; continue; }   /* safety */

        idx_t m = i - 1;            /* (n-1) */
        if (m == 1) { cache[i] = (seq_t)0; continue; }  /* no prime factors */

        idx_t limit = isqrt(m);
        ensure_primes_upto(limit);

        seq_t sum = 0;
        idx_t temp = m;
        for (size_t j = 0; j < primes_cnt && primes_arr[j] <= limit; ++j) {
            uint64_t p = primes_arr[j];
            if (temp % p == 0) {
                sum += cache[(i - 1) / p];
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) {                 /* remaining factor is prime > limit */
            sum += cache[(i - 1) / temp];
        }
        cache[i] = sum;
    }
    cache_cap = n;
}

/* ---------------------------------------------------------- */
/* public sequence function                                  */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
