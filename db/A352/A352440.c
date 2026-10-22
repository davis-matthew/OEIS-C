/*
 * A352440  Numbers k whose decimal expansion starts with the sum of the last
 *          three digits of k.
 * https://oeis.org/A352440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates numbers k whose decimal expansion starts with the sum of the last
   three decimal digits of k. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/digit_sum.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_len  = 0;      /* number of terms stored   */
    static size_t cache_cap  = 0;      /* allocated capacity       */
    static seq_t k           = (seq_t)100;   /* current candidate */

    if (n == 0) return (seq_t)0;   /* undefined below offset */

    /* grow cache if needed */
    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        /* generate missing terms */
        while (cache_len < (size_t)n) {
            seq_t last3 = k % (seq_t)1000;         /* last three digits */
            seq_t s     = digit_sum(last3);        /* sum of the last 3 digits */
            int d       = digit_len(k);            /* digits in k          */
            int l       = digit_len(s);            /* digits in sum        */
            idx_t expo  = (idx_t)(d - l);           /* exponent for pow10   */
            seq_t leading = k / pow10(expo);        /* leading part of k    */
            if (leading == s) {
                cache[cache_len++] = k;            /* store valid term     */
            }
            ++k;                                   /* next candidate       */
        }
    }
    return cache[n - 1];
}
