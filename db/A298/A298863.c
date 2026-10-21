/*
 * A298863  Ranks of primes p when all primes p and products 3*p are jointly
 *          ranked.
 * https://oeis.org/A298863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298863
   a(n) = rank of the n-th prime among the set {primes} ∪ {3·primes}
   Formula: a(n) = n + π(⌊pₙ/3⌋), where pₙ is the n‑th prime.   */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, calloc, realloc, free, abort */
#include <math.h>     /* log */

static uint8_t  *sieve     = NULL;   /* 0: composite, 1: potential prime   */
static size_t    sieve_size = 0;     /* number of entries in sieve         */

static uint64_t *primes    = NULL;   /* list of discovered primes          */
static size_t    primes_cnt = 0;     /* how many primes are stored         */
static size_t    primes_cap = 0;     /* capacity of the primes array       */

static seq_t *pi_arr  = NULL;        /* pi_arr[i] = number of primes ≤ i    */
static size_t   pi_cap = 0;          /* capacity of pi_arr                 */

/* ----------------------------------------------------------------- */
/* Estimate an upper bound for the n-th prime.                         */
static size_t upper_bound_prime(idx_t n)
{
    if (n < 6) return 15;
    double nn = (double)n;
    double l  = log(nn);
    double ll = log(l);
    double est = nn * (l + ll);
    return (size_t)est + 10;
}

/* ----------------------------------------------------------------- */
/* Build a new sieve up to (and including) 'limit'.                  */
static void build_sieve(size_t limit)
{
    free(sieve);
    free(primes);
    free(pi_arr);

    sieve = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!sieve) abort();

    /* initialise to 1 (prime candidate) */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* primes array */
    primes_cap = limit / 10 + 10;
    primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) abort();
    primes_cnt = 0;

    /* pi array */
    pi_cap = limit + 1;
    pi_arr = (seq_t*)malloc(pi_cap * sizeof(seq_t));
    if (!pi_arr) abort();

    seq_t cnt = 0;
    pi_arr[0] = 0;   /* for floor division cases */
    pi_arr[1] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = (uint64_t)i;
            cnt += (seq_t)1;
        }
        pi_arr[i] = cnt;
    }

    sieve_size = limit + 1;
}

/* ----------------------------------------------------------------- */
/* Ensure the global sieve covers at least 'limit'.                 */
static void ensure_sieve(size_t limit)
{
    if (limit + 1 <= sieve_size) return;   /* already sufficient */
    build_sieve(limit);
}

/* ----------------------------------------------------------------- */
/* main sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1 */

    /* Ensure enough primes are available.  The upper bound may be
       slightly larger than necessary; double if we are unlucky.   */
    size_t limit = upper_bound_prime((idx_t)n) + 10;
    while (1) {
        ensure_sieve(limit);
        if (primes_cnt >= (size_t)n) break;
        limit *= 2;                       /* grow limit opportunistically */
    }

    /* p_n: the n-th prime (0‑based index = n‑1) */
    uint64_t p_n = primes[n - 1];

    /* pi(p_n) = n (by definition of p_n) */
    /* pi(p_n / 3) – use integer division */
    size_t div = (size_t)(p_n / 3);
    seq_t pi_over3 = pi_arr[div];

    /* rank = pi(p_n) + pi(p_n / 3) */
    return (seq_t)n + pi_over3;
}
