/*
 * A266242  Numbers n such that the initial digit of the fractional part of
 *          n*Pi is 0.
 * https://oeis.org/A266242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the order‑statistics of integers n for which the first
 *            decimal digit of the fractional part of n*Pi equals 0
 * Range:     u8 n <= 23, u16 n <= 6549, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_pow.h"

/* Global cache of known terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 0;

/* Constants for the 64‑bit and big‑int builds */
static uint64_t pi_scaled_u64 = 31415926535897932ULL; /* floor(Pi*10^16) */
static const uint64_t shift64 = 1000000000000000ULL; /* 10^15 */

static seq_t pi_scaled_big = 0;
static seq_t shift_big     = 0;

static bool init_globals = false;

/* Grow cache capacity */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache     = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Compute the nth term of A266242.  1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for non‑offset */
    /* One‑time initialization of constants and first term */
    if (!init_globals) {
        init_globals = true;
        /* Big‑int constants (only used when seq_t is > 8 bytes) */
        pi_scaled_big = seq_from_str("31415926535897932");
        shift_big     = seq_pow((seq_t)10, 15);

        /* Allocate cache and insert the first term (0) */
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = 0;
        cache_len = 1;
        next_candidate = 1;   /* next number to test */
    }

    /* Generate terms until we have the desired index */
    while (cache_len < n) {
        seq_t digit;
        seq_t m_seq = (seq_t)next_candidate;

        if (sizeof(seq_t) > 8) {           /* big‑int build */
            seq_t prod  = m_seq * pi_scaled_big;
            seq_t div   = prod / shift_big;
            digit = div % 10;
        } else {                            /* 64‑bit build */
            __int128 prod = (__int128)m_seq * (__int128)pi_scaled_u64;
            uint64_t div  = (uint64_t)(prod / shift64);
            digit         = div % 10;
        }

        if (digit == 0) {
            grow_cache();
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
