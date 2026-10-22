/*
 * A300630  Positive numbers k without two consecutive ones in the binary
 *          representation of 1/k.
 * https://oeis.org/A300630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 268, u32 n <= 629, u64 n <= 629,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300630: a(n) = n-th positive integer k such that the
 * binary expansion of 1/k contains no consecutive 1's.
 * The implementation uses a simple sieve: numbers are tested
 * one by one until enough terms of the sequence have been found.
 * Each candidate k is accepted if the binary expansion of 1/k
 * (obtained by long division in base 2) never produces two
 * consecutive 1 bits.
 * The big integer arithmetic of the remainder is done in
 * unsigned 64‑bit variables to avoid overflows in the
 * seq_t type supplied by the build.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool passes(uint64_t k)
{
    /* Remove all factors of 2 – they do not affect the period. */
    while ((k & 1ULL) == 0ULL)
        k >>= 1;

    /* Powers of two always pass: 1/k terminates. */
    if (k == 1ULL)
        return true;

    uint64_t r = 1ULL;          /* current remainder       */
    bool prev_bit = false;      /* previous bit of 1/k     */
    while (true)
    {
        uint64_t tmp = r << 1;          /* numerator times 2      */
        bool bit = (tmp >= k);          /* next binary digit      */

        /* Two consecutive ones would violate the rule. */
        if (prev_bit && bit)
            return false;

        prev_bit = bit;
        r = tmp % k;
        if (r == 1ULL)                 /* cycle completed       */
            break;
    }
    return true;
}

/* -----------------  dynamic cache  ----------------- */
static seq_t *cache = NULL;         /* array of accepted k   */
static size_t cache_len = 0;        /* number of stored items */
static size_t cache_cap = 0;        /* allocated capacity    */
static uint64_t next_candidate = 1; /* next integer to test  */

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Expand the cache so that it contains at least n terms. */
static void extend_cache(size_t n)
{
    if (cache_len >= n)
        return;
    ensure_capacity(n);
    while (cache_len < n) {
        if (passes(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* -----------------  public routine  ----------------- */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined, but harmless */
        return (seq_t)0;
    extend_cache((size_t)n);
    return cache[n - 1];
}
