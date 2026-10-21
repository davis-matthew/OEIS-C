/*
 * A292599  a(1) = 0; for n > 1, a(n) = A010051(n) + 2*a(floor(n/2)).
 * https://oeis.org/A292599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010051(n) + 2*a(floor(n/2)), a(1)=0 where A010051(n) is
 *            1 iff n is prime, otherwise 0
 * Range:     u8 n <= 511, u16 n <= 9793, u32 n <= 9793, u64 n <= 9793,
 *            u128 n <= 9793, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Prime sieve cache */
static uint8_t *prime_flag = NULL;   /* prime_flag[i] == 1 if i is prime */
static size_t prime_cap = 0;             /* current size of prime_flag */

/* Ensure prime_flag is filled for all numbers up to n (inclusive). */
static void ensure_primes_upto(idx_t n)
{
    if ((size_t)n + 1 <= prime_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    uint8_t *new_arr = (uint8_t *)realloc(prime_flag, new_cap);
    if (!new_arr) /* out of memory; abort execution */
    {
        abort();
    }
    /* Initialise new part to 1 (prime) */
    memset(new_arr + prime_cap, 1, new_cap - prime_cap);

    new_arr[0] = new_arr[1] = 0;          /* 0 and 1 are not prime */
    for (size_t i = 2; i * i <= (size_t)n; ++i)
    {
        if (new_arr[i])
        {
            for (size_t j = i * i; j <= (size_t)n; j += i)
            {
                new_arr[j] = 0;
            }
        }
    }

    prime_flag = new_arr;
    prime_cap = new_cap;
}

/* Helper: return 1 if n is prime, otherwise 0. */
static seq_t is_prime(idx_t n)
{
    if ((size_t)n < prime_cap)
        return prime_flag[n];
    /* extend sieve to include n */
    ensure_primes_upto(n);
    return prime_flag[n];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Ensure we can test primality for the current n. */
    ensure_primes_upto(n);

    seq_t result = 0;
    seq_t power   = 1;           /* 2^k */

    while (n > 1)
    {
        result += power * (seq_t)is_prime(n);
        n >>= 1;                /* floor(n/2) */
        power <<= 1;            /* multiply by 2 */
    }

    return result;
}
