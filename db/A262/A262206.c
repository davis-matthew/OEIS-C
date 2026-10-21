/*
 * A262206  Product of prime(n) consecutive numbers starting from n.
 * https://oeis.org/A262206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) + n - 1)! / (n - 1)! Alternative: product of
 *            prime(n) consecutive numbers starting from n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static storage for primes */
static uint32_t *primes = NULL;   /* array of primes */
static size_t   primes_cnt = 0;   /* number of primes stored */
static size_t   primes_cap = 0;   /* capacity of the array */

/* Estimate an upper bound for the n-th prime (safe for n >= 6) */
static size_t nth_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;            /* covers first 5 primes */
    double nd = (double)n;
    double bound = nd * (log(nd) + log(log(nd))) + 3.0;
    return (size_t)bound;
}

/* Sieve of Eratosthenes up to 'limit', store primes in static array */
static void sieve_up_to(size_t limit)
{
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;  /* memory failure – unlikely in test bounds */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Reset static array and store primes */
    free(primes);
    primes     = NULL;
    primes_cnt = 0;
    primes_cap = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 128;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) return;  /* memory failure – unlikely */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* Ensure that we have at least 'n' primes computed */
static void ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n) return;
    size_t needed_limit = nth_prime_upper_bound(n);
    sieve_up_to(needed_limit);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe fallback */

    ensure_primes(n);            /* guarantees primes[0..n-1] exist */
    uint32_t p_n = primes[n - 1]; /* nth prime, 1‑based indexing */

    idx_t start = n;
    idx_t end   = n + (idx_t)p_n - 1;  /* inclusive */

    seq_t result = 1;
    for (idx_t i = start; i <= end; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
