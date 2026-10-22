/*
 * A335286  n is the a(n)-th positive integer having its sequence of exponents
 *          in canonical prime factorization.
 * https://oeis.org/A335286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1902, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A335286:
 *   a(n) = the ordinal of n among the positive integers having the same
 *   multiset of exponents in the prime signature of n.
 *   (the canonical prime‑factor exponent list of n).
 * For computing a(n) we factor n, then count all numbers ≤ n that have
 * the same exponent multiset by a recursive enumeration over primes.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64(…) */

static uint64_t *prime_list = NULL;   /* list of primes ≤ current limit   */
static size_t    prime_count = 0;     /* number of primes in prime_list    */
static uint64_t  prime_max   = 0;     /* maximum prime value stored         */

/* Simple sieve of Eratosthenes that builds primes up to 'limit'.   */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_max)
        return;

    /* free old list, if any */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;
    prime_max = 0;

    if (limit < 2)
        return;

    size_t sz = (size_t)(limit + 1);
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));   /* C++ cast required */
    if (!is_comp) return;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* count primes */
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++prime_count;

    /* allocate and store primes */
    if (prime_count) {
        prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
        if (!prime_list) {
            free(is_comp);
            return;
        }
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            prime_list[idx++] = i;

    prime_max = limit;
    free(is_comp);
}

/* Multiply seq_t by integer power (small exponent, no overflow in seq_t). */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* Recursive count of numbers ≤ limit having the exponent pattern 'exp' */
static seq_t dfs_count(const uint64_t *exp, size_t k, size_t pos,
                       size_t prime_start, seq_t prod, seq_t limit)
{
    if (pos == k)
        return 1;  /* a valid number found */

    seq_t count = 0;
    for (size_t i = prime_start; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        seq_t p_pow = pow_seq((seq_t)p, (int)exp[pos]);
        if (p_pow > limit)          /* further primes are larger → break */
            break;
        seq_t new_prod = prod * p_pow;
        if (new_prod > limit)       /* p_pow increases with p, break */
            break;
        count += dfs_count(exp, k, pos + 1, i + 1, new_prod, limit);
    }
    return count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined entry – safe return */

    /* factorize n */
    uint64_t n64  = (uint64_t)n;
    uint64_t pbuf[64], ebuf[64];
    size_t   pcnt = 0;
    factor_u64(n64, pbuf, ebuf, &pcnt);

    /* primes needed for enumeration during counting */
    ensure_primes_upto(n64);

    /* count numbers ≤ n with the same exponent pattern */
    return dfs_count(ebuf, pcnt, 0, 0, 1, (seq_t)n64);
}
