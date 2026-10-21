/*
 * A261916  Smallest p such that n can be written as n = p+q+r where
 *          p>=q>=r>=0 are palindromes.
 * https://oeis.org/A261916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 535, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_pal_dec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: finds the smallest palindrome p such that
   n = p + q + r with p >= q >= r >= 0 and all three palindromes. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_pal_dec.h"

/* Maximum number of palindromes we keep in the cache.
   For n up to ~1e6 this plenty, and far more than needed for n≈1e4. */
#define MAX_PALIN_COUNT 200000

/* Cache of decimal palindromes (stored as idx_t). */
static idx_t palins[MAX_PALIN_COUNT];
static size_t palin_cnt = 0;
static idx_t palin_max_generated = 0;

/* Make sure 0 is present in the cache. */
static void init_palins(void)
{
    if (palin_cnt == 0) {
        palins[0] = 0;
        palin_cnt = 1;
        palin_max_generated = 0;
    }
}

/* Extend the palindrome cache up to a new maximum value. */
static void extend_palins(idx_t new_max)
{
    if (new_max <= palin_max_generated) return;
    for (idx_t i = palin_max_generated + 1; i <= new_max; ++i) {
        if (is_pal_dec(i)) {
            if (palin_cnt < MAX_PALIN_COUNT)
                palins[palin_cnt++] = i;
        }
    }
    palin_max_generated = new_max;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_palins();                 /* Ensure 0 is cached */
    extend_palins(n);              /* Generate palindromes up to n */

    if (n == 0) return (seq_t)0;   /* trivial case */

    /* Search for the smallest qualifying p. */
    for (size_t pi = 0; pi < palin_cnt; ++pi) {
        idx_t p = palins[pi];
        if (p > n) break;                     /* p cannot exceed n */
        idx_t rest = n - p;                   /* need q + r = rest */

        /* Search for q and r. */
        for (size_t qi = 0; qi < palin_cnt; ++qi) {
            idx_t q = palins[qi];
            if (q > p) break;                 /* enforce p >= q */
            if (q > rest) break;              /* q cannot exceed rest */

            idx_t r = rest - q;
            if (r > q) continue;              /* enforce q >= r */
            if (is_pal_dec(r))                /* found a valid decomposition */
                return (seq_t)p;
        }
    }

    /* By the theorem every integer has such a representation;
       reaching here indicates an unexpected error. */
    return (seq_t)0;
}
