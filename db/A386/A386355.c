/*
 * A386355  Primes without {6, 8} as digits.
 * https://oeis.org/A386355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th prime that in base‑10 contains no digit 6 or 8.
 *            The routine builds primes incrementally and caches the filtered
 *            primes so that calls in any order are efficient
 * Range:     u8 n <= 47, u16 n <= 3134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hasForbiddenDigits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/hasForbiddenDigits.h"

/* global static tables for primes and filtered primes */
static uint64_t *prime_tbl   = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;

static seq_t *filtered_tbl  = NULL;
static size_t filtered_cnt   = 0;
static size_t filtered_cap   = 0;

static uint64_t next_candidate = 3;   /* next odd number to test */
static bool initialized = false;

/* grow prime_tbl if needed */
static void ensure_prime_cap(void)
{
    if (prime_cnt >= prime_cap)
    {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint64_t *tmp = (uint64_t*)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_tbl = tmp;
        prime_cap = new_cap;
    }
}

/* grow filtered_tbl if needed */
static void ensure_filtered_cap(void)
{
    if (filtered_cnt >= filtered_cap)
    {
        size_t new_cap = filtered_cap ? filtered_cap << 1 : 16;
        seq_t *tmp = (seq_t*)realloc(filtered_tbl, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        filtered_tbl = tmp;
        filtered_cap = new_cap;
    }
}

/* simple trial division primality test using prime_tbl */
static bool is_prime(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_tbl[i];
        if (p > n / p) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* initialisation: prime 2 and first filtered prime 2 */
static void init(void)
{
    ensure_prime_cap();
    prime_tbl[prime_cnt++] = 2;

    ensure_filtered_cap();
    filtered_tbl[filtered_cnt++] = (seq_t)2;

    next_candidate = 3;
    initialized = true;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (filtered_cnt >= n) return filtered_tbl[n - 1];

    /* generate primes until we have n filtered primes */
    while (filtered_cnt < n)
    {
        if (is_prime(next_candidate))
        {
            ensure_prime_cap();
            prime_tbl[prime_cnt++] = next_candidate;

            if (!hasForbiddenDigits((seq_t)next_candidate))
            {
                ensure_filtered_cap();
                filtered_tbl[filtered_cnt++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
    return filtered_tbl[n - 1];
}
