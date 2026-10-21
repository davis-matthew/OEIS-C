/*
 * A258155  Products of squares of three successive primes.
 * https://oeis.org/A258155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n * p_{n+1} * p_{n+2})^2
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 256, u128 n <= 10000,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/sieve_primes.h"

static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0;

static void ensure_primes(size_t need)
{
    if (prime_cache && prime_cnt >= need)
        return;
    if (prime_cache) {
        free(prime_cache);
        prime_cache = NULL;
        prime_cnt = 0;
    }

    idx_t limit;
    if (need < 6) {
        limit = 15;
    } else {
        double n = (double)need;
        double d = n * (log(n) + log(log(n))) + 10.0;
        limit = (idx_t)d + 1;
    }

    sieve_primes(limit, &prime_cache, &prime_cnt);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    size_t need = (size_t)(n + 2);
    ensure_primes(need);
    if (prime_cnt < need)
        return 0;
    idx_t p0 = prime_cache[(size_t)(n - 1)];
    idx_t p1 = prime_cache[(size_t)n];
    idx_t p2 = prime_cache[(size_t)(n + 1)];
    seq_t prod = (seq_t)p0;
    prod *= (seq_t)p1;
    prod *= (seq_t)p2;
    prod *= prod;
    return prod;
}
