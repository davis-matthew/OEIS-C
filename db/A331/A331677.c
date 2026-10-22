/*
 * A331677  a(n) is the difference between the number of primes smaller than
 *          prime(n) (i.e., n-1) and greater than prime(n) but less than
 *          2*prime(n).
 * https://oeis.org/A331677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n-1) - pi(2*prime(n))
 * Range:     i8 n <= 763, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state: primes list and its limit */
static uint32_t *primes = NULL;
static size_t    primes_cnt = 0;
static idx_t      sieve_limit = 0;

/* Forward declarations of internal helpers */
static void   ensure_sieve_limit(idx_t limit);
static void   ensure_n_primes(idx_t n);
static seq_t  count_primes_upto(idx_t x);

/* Generate primes up to and including `limit` (simple Eratosthenes) */
static void generate_primes_upto(idx_t limit)
{
    free(primes);

    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *arr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!arr) exit(EXIT_FAILURE);

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) arr[k++] = (uint32_t)i;

    free(is_prime);
    primes     = arr;
    primes_cnt = cnt;
    sieve_limit = limit;
}

/* Ensure the sieve covers up to at least `limit` */
static void ensure_sieve_limit(idx_t limit)
{
    if (limit <= sieve_limit) return;
    generate_primes_upto(limit);
}

/* Ensure at least `n` primes are available (prime(n)) */
static void ensure_n_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n) return;

    /* Rough estimate: number of primes < x is about x / ln x */
    /* Use a simple overestimate: 20*n is typically large enough */
    idx_t limit = n * 20;
    if (limit < 100) limit = 100;

    while ((size_t)n > primes_cnt) {
        ensure_sieve_limit(limit);
        if ((size_t)n <= primes_cnt) break;
        if (limit > SIZE_MAX / 2) break;     /* avoid overflow */
        limit *= 2;
    }
}

/* Binary search: how many primes are <= `x` */
static seq_t count_primes_upto(idx_t x)
{
    size_t low = 0, high = primes_cnt;
    while (low < high) {
        size_t mid = (low + high) / 2;
        if ((idx_t)primes[mid] <= x) low = mid + 1;
        else                         high = mid;
    }
    return (seq_t)low;
}

/* Public API: a(n) for OEIS A331677 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for offset 1 */

    ensure_n_primes(n);

    uint32_t p = primes[n-1];           /* prime(n) */

    idx_t double_p = (idx_t)((uint64_t)p * 2ULL);
    ensure_sieve_limit(double_p);

    seq_t pi_2p = count_primes_upto(double_p);
    seq_t nn = (seq_t)n;
    return (seq_t)(2 * nn - 1) - pi_2p;
}
