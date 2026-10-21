/*
 * A287117  Numbers with no odd prime binary proper prefixes.
 * https://oeis.org/A287117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1635, u32 n <= 2881, u64 n <= 2881,
 *            u128 n <= 2881, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287117 – Numbers with no odd prime binary proper prefixes */
/* Implements a(idx_t n) with 1‑based indexing.                     */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* ---------- Prime table for small numbers ---------- */
static const uint64_t PRIME_LIMIT = 1ULL << 22;      /* 4 194 304 */
static uint8_t *prime_flag = NULL;                  /* 1 = prime, 0 = composite */
static size_t prime_flag_size = 0;

static void init_prime_table(void)
{
    if (prime_flag) return;                         /* already built */

    prime_flag_size = PRIME_LIMIT + 1;
    prime_flag = (uint8_t*)calloc(prime_flag_size, 1);
    if (!prime_flag) abort();

    /* Assume every number >=2 is prime initially */
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) prime_flag[i] = 1;

    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (prime_flag[i])
        {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                prime_flag[j] = 0;                   /* composite */
        }
    }
}

/* ---------- Test whether a (small) odd number is prime ---------- */
static bool is_prefix_prime(seq_t x)
{
    if ((x & (seq_t)1ULL) == 0)                     /* even → cannot be an odd prime */
        return false;

    uint64_t up = (uint64_t)x;
    if (up <= PRIME_LIMIT)
        return prime_flag[up] != 0;
    return is_prime_big(x);
}

/* ---------- Check if any proper binary prefix of x is an odd prime ---------- */
static bool has_odd_prime_prefix(seq_t x)
{
    seq_t p = x >> 1;                               /* drop the least significant bit */
    while (p > (seq_t)0)
    {
        if ((p & (seq_t)1) != 0 && is_prefix_prime(p))
            return true;
        p >>= 1;
    }
    return false;
}

/* ---------- Cache for already computed terms ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* out of bounds */
    init_prime_table();

    while (cache_len < (size_t)n)
    {
        if (!has_odd_prime_prefix(next_candidate))
        {
            if (cache_len >= cache_cap) grow_cache();
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
