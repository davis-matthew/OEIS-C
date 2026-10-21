/*
 * A294996  Numbers m such that the largest digit of m^3 is 6.
 * https://oeis.org/A294996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 689, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_digit_is_six_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer m such that the largest digit
   of m³ is 6.  The values are generated on demand and cached; the
   implementation works for any seq_t (uint8_t, uint16_t, …, unsigned
   __int128 or arbitrary precision).  The sequence has no simple
   closed form. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_digit_is_six_uint128.h"

/* Utility supplied by the build – used only for very large values of
   seq_t; the cube is computed directly in the implementation below. */
extern seq_t big_pow(seq_t base, seq_t exp);

/* ------------------------------------------------------------------
   Cache for the sequence values.  The cache is grown geometrically. */
static seq_t *cache      = NULL;   /* array of found terms          */
static size_t cache_cap  = 0;      /* allocated capacity of cache   */
static size_t cache_len  = 0;      /* number of terms currently stored */
static idx_t  curr_m     = 1;      /* next candidate m to test       */

static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t cap = cache_cap ? cache_cap : 16;
    while (cap < new_cap)
        cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failure */

    cache = tmp;
    cache_cap = cap;
}

/* ------------------------------------------------------------------
   Return the n‑th term of the sequence (1‑based index).  Calls may
   arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined – keep the call safe */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Search for the next valid `m`. */
        while (true) {
            unsigned __int128 cube =
                (__uint128_t)curr_m * curr_m * curr_m;
            if (largest_digit_is_six_uint128(cube)) {
                /* Store the found term. */
                if (cache_len == cache_cap)
                    grow_cache(cache_cap ? cache_cap << 1 : 16);
                cache[cache_len++] = (seq_t)curr_m;
                break;                 /* next candidate m continues after loop */
            }
            curr_m++;                     /* next candidate m */
        }
        curr_m++;                         /* advance past the found value */
    }

    return cache[(size_t)n - 1];
}
