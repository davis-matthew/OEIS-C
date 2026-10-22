/*
 * A349273  Number of odd divisors of prime(n) - 1.
 * https://oeis.org/A349273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odd divisors of prime(n) - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to 200 000
 * (enough for the first 10 000 primes) using a sieve.
 * For a given n it retrieves the n‑th prime, removes all
 * factors of 2 from (p‑1), factors the remaining odd part
 * by trial division on the precomputed primes, and returns
 * the divisor count of that odd part. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the prime table */
#define MAX_PRIMES 20000
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

/* Build the sieve and fill the primes array. */
static void init_sieve(void)
{
    if (prime_count > 0)        /* already initialised */
        return;

    const idx_t limit = 200000;      /* > 10 000th prime (104 729)  */
    bool *is_comp = (bool *)malloc((size_t)limit + 1);
    if (!is_comp)                 /* allocation failure – abort */
        exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= limit; ++i)
        is_comp[i] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_comp[m] = true;
        }
    }

    for (idx_t i = 2; i <= limit && prime_count < MAX_PRIMES; ++i) {
        if (!is_comp[i])
            primes[prime_count++] = (uint32_t)i;
    }

    free(is_comp);
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                     /* out of OEIS bounds, not used */
        return (seq_t)0;

    init_sieve();                   /* guarantee primes ready */

    if (n > prime_count)            /* safety for unexpected n */
        return (seq_t)0;

    uint64_t p = primes[n - 1];      /* nth prime, 1‑based index */
    uint64_t m = p - 1;

    /* strip all factors of 2 */
    while ((m & 1ULL) == 0 && m != 0)
        m >>= 1;

    seq_t result = 1;
    uint64_t remainder = m;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t q = primes[i];
        if ((uint64_t)q * (uint64_t)q > remainder)
            break;

        if (remainder % q == 0) {
            uint64_t exp = 0;
            while (remainder % q == 0) {
                remainder /= q;
                ++exp;
            }
            result *= (seq_t)(exp + 1);
        }
    }

    /* if a prime factor > sqrt(m) remains */
    if (remainder > 1)
        result *= (seq_t)2;

    return result;
}
