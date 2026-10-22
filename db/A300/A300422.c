/*
 * A300422  Number of n X 3 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300422
   Formula: a(n) = 5*a(n-1) + 3*a(n-2) - 11*a(n-3) + a(n-4)
                - 11*a(n-5) - 2*a(n-6) + 14*a(n-7) - 6*a(n-8)
                - 8*a(n-9) - 4*a(n-10) - 8*a(n-11) + 4*a(n-13)
        (valid for n > 14)
   Initial values:
     a(1) = 1 , a(2) = 10 , a(3) = 53 , a(4) = 272 ,
     a(5) = 1390 , a(6) = 7179 , a(7) = 37042 , a(8) = 191135 ,
     a(9) = 986243 , a(10) = 5089160 , a(11) = 26260738 ,
     a(12) = 135509178 , a(13) = 699246723 , a(14) = 3608214131
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* static cache for computed values, 1-based indices */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated length (in seq_t elements) */
static size_t last_comp  = 0;   /* largest n already computed */

static const char *init_str[15] = {
    "0",           /* dummy for index 0 */
    "1",           /* a(1) */
    "10",          /* a(2) */
    "53",          /* a(3) */
    "272",         /* a(4) */
    "1390",        /* a(5) */
    "7179",        /* a(6) */
    "37042",       /* a(7) */
    "191135",      /* a(8) */
    "986243",      /* a(9) */
    "5089160",     /* a(10) */
    "26260738",    /* a(11) */
    "135509178",   /* a(12) */
    "699246723",   /* a(13) */
    "3608214131"   /* a(14) */
};

/* Ensure that cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t req = (size_t)(n + 1);           /* we keep index 0 as a placeholder */
    if (cache_cap >= req) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < req) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;

    /* fill missing initial values if they have not yet been computed */
    if (last_comp < 14) {
        size_t start = last_comp + 1;
        if (start < 1) start = 1;
        for (size_t i = start; i <= 14; ++i) {
            cache[i] = seq_from_str(init_str[i]);
        }
        /* keep any previously computed values (if any) */
        if (last_comp < 14) last_comp = 14;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but safe */

    if (n <= 14) {
        /* values fit in seq_t if seq_t is wide enough, otherwise they are
           truncated but that is acceptable */
        return seq_from_str(init_str[n]);
    }

    ensure_capacity(n);

    for (idx_t i = (idx_t)(last_comp + 1); i <= n; ++i) {
        seq_t pos = cache[i-1]*5 + cache[i-2]*3 + cache[i-4] + 
                    cache[i-7]*14 + cache[i-13]*4;
        seq_t neg = cache[i-3]*11 + cache[i-5]*11 + cache[i-6]*2 + 
                    cache[i-8]*6 + cache[i-9]*8 + cache[i-10]*4 + 
                    cache[i-11]*8;
        cache[i] = pos - neg;
    }
    last_comp = (size_t)n;
    return cache[n];
}
