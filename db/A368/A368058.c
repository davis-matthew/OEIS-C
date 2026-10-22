/*
 * A368058  Sum of the smaller parts of the partitions of n into two distinct
 *          parts with larger part prime.
 * https://oeis.org/A368058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..⌊(n-1)/2⌋} i * prime(n-i) (prime ≡ 1 if
 *            argument is prime, 0 otherwise)
 * Range:     u8 n <= 98, u16 n <= 1928, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static prime sieve cache */
static uint8_t *prime_flag = NULL; /* 0 = prime, 1 = composite */
static idx_t prime_cap  = 0;

/* extend prime_flag to cover all indices up to n (inclusive) */
static void _ensure_prime_sieve(idx_t n)
{
    if (n <= prime_cap)
        return;

    /* allocate new flag array */
    size_t size = (size_t)(n + 1);
    uint8_t *flags = (uint8_t *)calloc(size, 1);
    if (!flags)
        exit(1); /* allocation failure */

    /* mark 0 and 1 as composite */
    if (size > 0) flags[0] = 1;
    if (size > 1) flags[1] = 1;

    /* sieve */
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (flags[p] == 0) /* p is still prime */
        {
            for (idx_t m = p * p; m <= n; m += p)
                flags[m] = 1;
        }
    }

    /* replace old cache */
    free(prime_flag);
    prime_flag = flags;
    prime_cap = n;
}

/* sequence definition */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0; /* a(1) = 0 as per OEIS definition */

    _ensure_prime_sieve(n - 1);          /* we need primes up to n-1 */

    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i)
    {
        idx_t larger = n - i;            /* larger part of the partition */
        if (larger >= 2 && prime_flag[larger] == 0) /* larger is prime */
            sum += (seq_t)i;
    }
    return sum;
}
