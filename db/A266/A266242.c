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
 * Validated: standard, pass, 69 terms through n = 9922 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

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
        cache     = mem_resize(seq_t, cache, cache_cap);
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
        shift_big     = ipow_idx((seq_t)10, 15);

        /* Allocate cache and insert the first term (0) */
        cache_cap = 64;
        cache     = mem_new(seq_t, cache_cap);
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
