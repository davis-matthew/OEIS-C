/*
 * A276971  Odd integers n such that 2^n == 2^11 (mod n).
 * https://oeis.org/A276971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276971: search for odd n such that 2^n == 2^11 (mod n) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 3; /* odd numbers to test after the first term 1 */

/* Allocate more space in the cache if needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Multiply two 64‑bit values modulo mod.  Uses 128‑bit arithmetic. */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Fast modular exponentiation (base^exp mod mod).  Assumes exp fits in 64 bits. */
static inline uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1)
        return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = mul_mod_uint64(result, b, mod);
        b = mul_mod_uint64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Compute the n-th term of the sequence.  a(1) = 1, a(2) = 3, ...          */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, defensive */

    /* Add the first term explicitly on first call */
    if (cache_size == 0)
    {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_size = 1;
        next_candidate = 3;
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n)
    {
        idx_t cand = next_candidate;
        next_candidate += 2;          /* only odd numbers */

        /* 2^cand mod cand */
        uint64_t left = powmod_uint64(2, cand, (uint64_t)cand);

        /* 2^11 mod cand (or 2048 if cand > 2048) */
        uint64_t right;
        if (cand > 2048ULL)
            right = 2048ULL % (uint64_t)cand; /* always 2048 when cand>2048 */
        else
            right = powmod_uint64(2, 11, (uint64_t)cand);

        if (left == right)
        {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
