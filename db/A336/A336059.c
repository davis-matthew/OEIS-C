/*
 * A336059  Numbers p such that p, 2p-1, 3p-2, 4p-3 are primes.
 * https://oeis.org/A336059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 8, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336059: numbers p such that p, 2p‑1, 3p‑2, 4p‑3 are prime */
/* Computed by sieving primes up to a fixed limit and testing the conditions. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Fix the sieve upper bound – enough for the first 10 000 terms. */
static const idx_t SIEVE_LIMIT = 50000000ULL;

/* Global state: bitset of composites, list of primes. These are built once. */
static unsigned char *sieve      = NULL;          /* bit array of composites */
static uint32_t   *prime_list   = NULL;          /* list of primes */
static size_t       prime_cnt   = 0;              /* number of primes stored */
static bool         sieve_built = false;

/* Helper macros for the bitset. */
#define BIT_SET(arr, idx)   ((arr)[(idx)>>3] |=  (1U<<((idx)&7)))
#define BIT_GET(arr, idx)   ((arr)[(idx)>>3] &   (1U<<((idx)&7)))

/* Build the sieve of Eratosthenes up to SIEVE_LIMIT and collect primes. */
static void build_sieve(void)
{
    idx_t  limit = SIEVE_LIMIT;
    size_t byte_len = (size_t)(limit >> 3) + 1;

    sieve = (unsigned char *)malloc(byte_len);
    if (!sieve) exit(EXIT_FAILURE);
    memset(sieve, 0, byte_len);

    /* mark 0 and 1 as composite */
    BIT_SET(sieve, 0);
    BIT_SET(sieve, 1);

    /* sieve */
    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i)
    {
        if (!BIT_GET(sieve, i))
        {
            for (idx_t j = i * i; j <= limit; j += i)
                BIT_SET(sieve, j);
        }
    }

    /* allocate a rough upper bound for the prime count */
    size_t est_cap = (size_t)(limit / 10) + 100;   /* safe overestimate */
    prime_list = (uint32_t *)malloc(est_cap * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t cap = est_cap;
    size_t cnt = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!BIT_GET(sieve, i))
        {
            if (cnt == cap)
            {
                cap <<= 1;   /* double capacity */
                prime_list = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[cnt++] = (uint32_t)i;
        }
    }
    prime_cnt = cnt;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (!sieve_built)
    {
        build_sieve();
        sieve_built = true;
    }

    idx_t found = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = (seq_t)prime_list[i];

        /* Check the four required conditions. */
        if (!is_prime_big(p * 2 - (seq_t)1))          continue;
        if (!is_prime_big(p * 3 - (seq_t)2))          continue;
        if (!is_prime_big(p * 4 - (seq_t)3))          continue;

        ++found;
        if (found == n)
            return p;
    }

    /* If the sieve range was insufficient, continue searching without a sieve. */
    seq_t candidate = (seq_t)prime_list[prime_cnt - 1] + (seq_t)1;
    for (;; ++candidate)
    {
        if (!is_prime_big(candidate)) continue;

        if (!is_prime_big(candidate * 2 - (seq_t)1))          continue;
        if (!is_prime_big(candidate * 3 - (seq_t)2))          continue;
        if (!is_prime_big(candidate * 4 - (seq_t)3))          continue;

        ++found;
        if (found == n)
            return candidate;
    }
}
