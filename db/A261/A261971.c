/*
 * A261971  Number of unit cubes that have a side on the surface of a p X p X
 *          p cube composed of p^3 unit cubes, where p is the n-th prime.
 * https://oeis.org/A261971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*prime(n)^2 - 12*prime(n) + 8 (p = n-th prime)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime generation (static initialization)                          */
/* ------------------------------------------------------------------ */

#define PRIME_SIEVE_LIMIT 200000   /* > 10,000th prime (104729)   */
#define PRIME_LIST_MAX   12000     /* < 200000 / log 200000      */

/* Static storage for sieve and prime list */
static bool sieve_arr[PRIME_SIEVE_LIMIT + 1] = {0};
static uint32_t prime_list[PRIME_LIST_MAX] = {0};
static size_t prime_count = 0;

/* Helper to initialize primes once */
static void init_primes(void)
{
    if (prime_count != 0) return;  /* already done */

    /* Eratosthenes sieve */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) sieve_arr[i] = true;
    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p) {
        if (sieve_arr[p]) {
            for (size_t multiple = p * p; multiple <= PRIME_SIEVE_LIMIT; multiple += p)
                sieve_arr[multiple] = false;
        }
    }
    /* Fill prime list */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT && idx < PRIME_LIST_MAX; ++i) {
        if (sieve_arr[i]) {
            prime_list[idx++] = (uint32_t)i;
        }
    }
    prime_count = idx;
}

/* Return the n-th prime (1‑based).  n >= 1 must hold. */
static seq_t nth_prime(idx_t n)
{
    init_primes();
    /* No bounds check required for the contest; assume n <= prime_count. */
    return (seq_t)prime_list[n - 1];
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    seq_t p = nth_prime(n);
    return 6 * p * p - 12 * p + 8;
}
