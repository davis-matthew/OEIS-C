/*
 * A392359  Denominators of the reduced fractions of the ratios of the number
 *          of primes less than n over n.
 * https://oeis.org/A392359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of pi(n)/n in reduced form i.e. a(n) = n /
 *            gcd(n, pi(n)) where pi(n) = #{primes ≤ n}
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint32_t *primes = NULL;         /* list of primes up to max_sieve */
static size_t primes_cnt = 0;
static idx_t max_sieve = 0;

/* Ensure that all primes up to at least n are generated */
static void ensure_primes_until(idx_t n)
{
    if (n <= max_sieve)
        return;

    /* Simple Sieve of Eratosthenes up to n */
    idx_t limit = n;
    bool *isprime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!isprime)
        return; /* allocation failure – abort silently */

    for (idx_t i = 0; i <= limit; ++i)
        isprime[i] = true;
    if (limit >= 0) isprime[0] = false;
    if (limit >= 1) isprime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (isprime[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                isprime[mult] = false;
        }
    }

    /* Count primes and build the new array */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isprime[i])
            ++count;

    uint32_t *new_primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_primes) {
        free(isprime);
        return; /* allocation failure – abort silently */
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isprime[i])
            new_primes[pos++] = (uint32_t)i;

    free(isprime);

    /* Replace old data */
    free(primes);
    primes = new_primes;
    primes_cnt = count;
    max_sieve = limit;
}

/* Count number of primes ≤ limit using binary search */
static size_t count_primes_upto(uint32_t limit)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 1 to avoid division by zero */
        return (seq_t)1;
    }

    ensure_primes_until(n);

    seq_t pi = (seq_t)count_primes_upto((uint32_t)n);
    seq_t denom = (seq_t)n / gcd((seq_t)n, pi);
    return denom;
}
