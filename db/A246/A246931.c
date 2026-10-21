/*
 * A246931  Prime(14*n).
 * https://oeis.org/A246931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 467, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine implements a sieve of Eratosthenes to compute the
 * 14×n‑th prime by generating all primes up to a safe ceiling.
 * The primes are cached statically so that repeated calls for
 * arbitrary indices are fast.  The function returns the 14*n‑th
 * prime as a seq_t, converting from an internal 64‑bit array.
 * The code is valid both in C11 and in C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of computed prime values, stored as 64‑bit integers. */
static uint64_t *prime_vals = NULL;
static size_t prime_cnt   = 0;   /* number of primes in the cache */
static idx_t  sieve_limit = 0;   /* maximum number sieved so far */

/* Ensure the cache contains at least 'target' primes. */
static void ensure_primes(idx_t target)
{
    if (target <= prime_cnt) return;          /* already have enough */

    /* Rough upper bound: the 14*n‑th prime < n*20 for n ≤ 10 000
     * (the 140 000‑th prime is < 3 M).  Use a small extra margin.   */
    idx_t new_limit = target * 20 + 100;
    if (new_limit <= sieve_limit) new_limit = sieve_limit + 100;

    /* Allocate boolean sieve array.  Cast the void* from calloc. */
    bool *is_composite = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!is_composite) { /* allocate failure: abort */ exit(EXIT_FAILURE); }

    /* Classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= new_limit; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_composite[j] = true;

    /* Count primes up to new_limit to know array size. */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!is_composite[i]) ++new_cnt;

    /* Allocate or reallocate cache for the new number of primes. */
    uint64_t *new_vals = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
    if (!new_vals) { free(is_composite); exit(EXIT_FAILURE); }

    /* Fill array with the detected primes. */
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!is_composite[i]) new_vals[pos++] = (uint64_t)i;

    /* Clean up old data. */
    free(is_composite);
    free(prime_vals);

    prime_vals = new_vals;
    prime_cnt  = new_cnt;
    sieve_limit = new_limit;
}

/* Requested OEIS sequence: a(n) = Prime(14*n)   (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1: a(1)=43 */
    idx_t target = n * 14;              /* 14*n-th prime required */

    ensure_primes(target);              /* grow cache if needed */

    /* Cast the stored 64‑bit value to seq_t (works for integral and
     * arbitrary‑precision types – the constructor or conversion
     * operator handles the cast). */
    return (seq_t)prime_vals[target - 1];
}
