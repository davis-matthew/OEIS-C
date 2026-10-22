/*
 * A386176  Primes having only {3, 4, 7, 8} as digits.
 * https://oeis.org/A386176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence: A386176    
    Primes having only {3, 4, 7, 8} as decimal digits
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"      /* isPrimeGeneric(seq_t n) */
#include "../utils/append_candidate.h"    /* append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val) */
#include "../utils/ensure_capacity.h"     /* ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) */

static seq_t *cache = NULL;      /* cached primes of the sequence     */
static size_t cache_len = 0;      /* number of elements in cache       */
static size_t cache_cap = 0;      /* capacity of cache array           */

/* Current state of the incremental generator */
static idx_t  current_len = 1;                    /* current number of digits      */
static uint64_t counter       = 0;                 /* index of the next candidate   */
static uint64_t total_counters = 4;                 /* 4^current_len                */
static unsigned char digits[32];                   /* temporary digit storage      */
static const unsigned char digit_map[4] = {3, 4, 7, 8};

/* Ensures that at least `n` terms are in the cache */
static void
ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

    while (cache_len < (size_t)n) {
        /* if counter exceeded current length, move to next length */
        if (counter >= total_counters) {
            ++current_len;
            counter = 0;
            total_counters = 1;
            for (unsigned i = 0; i < current_len; ++i)
                total_counters *= 4;           /* 4^current_len */
        }

        /* Build the candidate number from base-4 counter */
        uint64_t tmp = counter;
        for (unsigned i = 0; i < current_len; ++i) {
            unsigned idx = (unsigned)(tmp % 4);
            tmp /= 4;
            digits[i] = digit_map[idx];
        }

        /* Convert digits (least significant first) to numeric seq_t */
        seq_t x = 0;
        for (int i = (int)current_len - 1; i >= 0; --i)
            x = x * 10 + digits[i];

        /* Advance counter for next iteration */
        ++counter;

        /* Test primality and cache if prime */
        if (isPrimeGeneric(x))
            append_candidate(&cache, &cache_len, &cache_cap, x);
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based in OEIS; convert to 0‑based index in code */
    if (n == 0)
        return 0;          /* undefined, but guard against accidental call */

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
