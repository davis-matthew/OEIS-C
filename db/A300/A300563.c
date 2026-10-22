/*
 * A300563  Condensed deep factorization of n, A300562(n) written in decimal:
 *          floor of odd part of A300561(n) divided by 2.
 * https://oeis.org/A300563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( odd_part(A300561(n)) / 2 )
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of sequence A300563 (Condensed deep factorization of n) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Constants for prime sieve */
#define PRIME_MAX 200000

/* Global storage for primes */
static uint64_t primes_array[PRIME_MAX / 2 + 10];
static size_t   primes_count   = 0;
static idx_t    prime_limit    = 0;
static bool     is_prime[PRIME_MAX + 1];

/* Ensure that the prime tables contain all primes up to limit */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (limit > PRIME_MAX)
        limit = PRIME_MAX;   /* safety, not expected for the problem */

    for (size_t i = 2; i <= (size_t)limit; ++i)
        is_prime[i] = true;

    for (size_t i = 2; (size_t)(i * i) <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;
        }
    }

    primes_count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            primes_array[primes_count++] = i;

    prime_limit = limit;
}

/* Recursive routine that appends the binary digits of a number's
   deep factorization to *acc.  *acc is updated in place. */
static void encode_number(idx_t m, seq_t *acc)
{
    if (m <= 1)
        return;

    idx_t remaining = m;
    for (size_t pi = 0;
         pi < primes_count && (uint64_t)primes_array[pi] * primes_array[pi] <= remaining;
         ++pi)
    {
        uint64_t p = primes_array[pi];
        if (remaining % p == 0) {
            int exponent = 0;
            while (remaining % p == 0) {
                remaining /= p;
                exponent++;
            }

            /* '[' for the prime factor */
            *acc = (*acc) * 2 + 1;

            /* encode the prime index (pi + 1) */
            encode_number((idx_t)(pi + 1), acc);

            /* '[' for the exponent */
            *acc = (*acc) * 2 + 1;

            /* encode the exponent value */
            encode_number((idx_t)exponent, acc);

            /* ']' */
            *acc = (*acc) * 2 + 0;

            /* ']' */
            *acc = (*acc) * 2 + 0;
        }
    }

    /* Remaining part is a prime > sqrt(m) */
    if (remaining > 1) {
        uint64_t p = remaining;
        size_t pi = 0;
        while (pi < primes_count && primes_array[pi] != p)
            ++pi;

        /* '[' for the prime factor */
        *acc = (*acc) * 2 + 1;

        /* encode the prime index */
        encode_number((idx_t)(pi + 1), acc);

        /* '[' for the exponent (which is 1) */
        *acc = (*acc) * 2 + 1;
        /* exponent 1 has empty encoding */

        /* ']' */
        *acc = (*acc) * 2 + 0;

        /* ']' */
        *acc = (*acc) * 2 + 0;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0) {
        seq_t zero = 0;
        return zero;
    }

    /* Pre‑compute primes up to n (enough for all values we will factor) */
    ensure_primes_upto(n);

    /* Build the binary number for the deep factorization of n */
    seq_t x = 0;
    encode_number(n, &x);

    if (x == 0)
        return 0;          /* a(1) = 0 */

    /* Find v = v₂(x), the exponent of 2 in x */
    idx_t v = 0;
    seq_t temp = x;
    while ((temp & 1) == 0) {
        temp = temp / 2;
        v++;
    }

    /* a(n) = floor( odd_part(x)/2 ) = x >> (v+1) */
    seq_t result = x >> (long long)(v + 1);
    return result;
}
