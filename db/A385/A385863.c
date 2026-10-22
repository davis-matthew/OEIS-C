/*
 * A385863  a(n) is the largest number of distinct prime factors a number with
 *          at most n digits can have.
 * https://oeis.org/A385863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = largest k such that primorial(k) < 10^n (primorial(k) =
 *            product of the first k primes)
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* Static state for the prime sieve */
static uint64_t *primes   = NULL;
static double  *cum_log  = NULL;
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

/* Ensure the dynamic arrays have enough capacity */
static void ensure_prime_capacity(void)
{
    if (prime_cnt < prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    primes  = (uint64_t*)realloc(primes,  new_cap * sizeof(*primes));
    cum_log = (double*)realloc(cum_log, new_cap * sizeof(*cum_log));
    prime_cap = new_cap;
}

/* Return the next prime greater than the last stored prime */
static uint64_t next_prime(void)
{
    uint64_t cand = (prime_cnt == 0) ? 2ULL : primes[prime_cnt-1] + 1ULL;
    if (cand > 2 && cand % 2ULL == 0) cand++;        /* skip even numbers */

    for (;; cand += (cand == 2 ? 1 : 2)) {           /* 2 then odd increments */
        uint64_t lim = (uint64_t)(sqrt((double)cand) + 1.0);
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i) {
            if (cand % primes[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) return cand;
    }
}

/* Generate the next prime and update cumulative log10 sum */
static void generate_next(void)
{
    ensure_prime_capacity();
    uint64_t p = next_prime();
    primes[prime_cnt] = p;
    double next_log = (prime_cnt == 0) ? log10((double)p)
                                       : cum_log[prime_cnt-1] + log10((double)p);
    cum_log[prime_cnt] = next_log;
    prime_cnt++;
}

/* Make sure we have generated primes until cum_log[prime_cnt-1] > n */
static void ensure_primes_for_n(idx_t n)
{
    double dn = (double)n;
    while (prime_cnt == 0 || cum_log[prime_cnt-1] <= dn)
        generate_next();
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Edge case: n must be >= 1 (offset 1). For n < 1 we return 0. */
    if (n < 1) return (seq_t)0;

    ensure_primes_for_n(n);
    /* cum_log[prime_cnt-1] is the first cumulative log exceeding n,
       so the largest k with primorial(k) < 10^n is prime_cnt-1. */
    return (seq_t)(prime_cnt - 1);
}
