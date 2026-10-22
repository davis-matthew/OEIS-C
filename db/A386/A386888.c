/*
 * A386888  Number of ways to write n as u + (1+(n mod 2))*v with v <= n/2,
 *          where u and v are both sums of three consecutive primes.
 * https://oeis.org/A386888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of pairs (u,v) such that v <= n/2, v and u are sums of
 *            three consecutive primes, and n = u + (1 + (n mod 2))*v
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached primes up to prime_limit */
static uint32_t *primes_ptr = NULL;
static size_t   primes_cnt  = 0;
static idx_t    prime_limit = 0;

/* Compute all primes <= limit and store them in the static cache. */
static void compute_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)    /* already enough */
        return;

    /* free previous cache */
    if (primes_ptr) {
        free(primes_ptr);
        primes_ptr = NULL;
        primes_cnt = 0;
    }

    /* sieve */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)
        return;                /* out of memory – caller will handle */

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0)
        is_prime[0] = false;
    if (limit >= 1)
        is_prime[1] = false;

    idx_t root = 0;
    while ((root + 1) * (root + 1) <= limit)
        ++root;

    for (idx_t p = 2; p <= root; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* store primes */
    primes_ptr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes_ptr) {
        free(is_prime);
        return;    /* out of memory – caller will handle */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes_ptr[idx++] = (uint32_t)i;

    primes_cnt  = cnt;
    prime_limit = limit;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* offset starts at 1 */

    /* factor 1 for even, 2 for odd n */
    idx_t factor = (n % 2) ? 2 : 1;

    /* we need primes up to at least n, because all sums <= n */
    compute_primes_upto(n);

    /* array marking sums of three consecutive primes up to n */
    uint8_t *sum_flag = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!sum_flag)
        return (seq_t)0;           /* out of memory – return zero */

    /* mark all sums of three consecutive primes <= n */
    if (primes_cnt >= 3) {
        for (size_t i = 0; i + 2 < primes_cnt; ++i) {
            idx_t s = (idx_t)primes_ptr[i] + (idx_t)primes_ptr[i + 1] + (idx_t)primes_ptr[i + 2];
            if (s > n)
                break;           /* later sums are larger */
            sum_flag[s] = 1;
        }
    }

    seq_t count = 0;
    idx_t max_v = n / 2;

    for (idx_t v = 0; v <= max_v; ++v) {
        if (!sum_flag[v])
            continue;

        idx_t u = n - factor * v;
        if (u > n)      /* should not happen, but guard */
            continue;
        if (sum_flag[u])
            ++count;
    }

    free(sum_flag);
    return count;
}
