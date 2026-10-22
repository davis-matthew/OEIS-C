/*
 * A336940  Number of odd divisors of n!.
 * https://oeis.org/A336940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors of n!
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 60, u64 n <= 136,
 *            u128 n <= 310, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

static idx_t  prime_limit = 0;
static idx_t *prime_list  = NULL;
static size_t prime_count = 0;

/* Compute the Legendre exponent of prime p in n! */
static idx_t legendre_exp(idx_t n, idx_t p)
{
    idx_t exp = 0;
    idx_t div = p;
    while (div <= n) {
        exp += n / div;
        /* prevent overflow of div *= p */
        if (div > n / p) break;
        div *= p;
    }
    return exp;
}

seq_t a(idx_t n)
{
    /* n = 0 or 1: 0! = 1! = 1 has exactly one odd divisor */
    if (n < 2) return (seq_t)1;

    /* ensure primes up to n are available */
    if (n > prime_limit) {
        /* free old list if present */
        free(prime_list);
        prime_list  = NULL;
        prime_count = 0;
        build_primes(n, &prime_list, &prime_count);
        prime_limit = n;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p == 2) continue;        /* skip even prime */
        idx_t e   = legendre_exp(n, p);
        seq_t mult = (seq_t)(e + 1); /* factor contributes e+1 */
        result *= mult;
    }
    return result;
}
