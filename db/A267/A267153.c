/*
 * A267153  Binary representation of the n-th iteration of the "Rule 107"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10000*a(n-2) + a(n-4) - 10000*a(n-6) for n>12
   Initial values (n=0..12):
   a(0)  = 1
   a(1)  = 100
   a(2)  = 1011
   a(3)  = 1101110
   a(4)  = 1110111
   a(5)  = 11110111010
   a(6)  = 111011011
   a(7)  = 111111011111110
   a(8)  = 11100000111
   a(9)  = 1111111101011111010
   a(10) = 1101100011011
   a(11) = 11111111111111011111110
   a(12) = 11100000111
*/
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

/* constant strings for the first 13 terms */
static const char *const init_str[13] = {
    "1",                      /* a(0)  */
    "100",                    /* a(1)  */
    "1011",                   /* a(2)  */
    "1101110",                /* a(3)  */
    "1110111",                /* a(4)  */
    "11110111010",            /* a(5)  */
    "111011011",              /* a(6)  */
    "111111011111110",        /* a(7)  */
    "11100000111",            /* a(8)  */
    "1111111101011111010",    /* a(9)  */
    "1101100011011",          /* a(10) */
    "11111111111111011111110",/* a(11) */
    "11100000111"             /* a(12) */
};

/* cache for computed values */
static seq_t *cache = NULL;
static idx_t  cache_cap   = 0;   /* number of slots currently allocated */
static idx_t  max_computed = 12; /* highest index whose value is stored */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* allocate or enlarge cache to hold indices up to n */
    size_t needed_cap = n + 1;
    if (needed_cap < 13) needed_cap = 13;

    if (cache == NULL) {
        /* initial allocation */
        cache = (seq_t *)malloc(needed_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        /* set initial 13 terms */
        for (size_t i = 0; i < 13; ++i) {
            cache[i] = seq_from_str(init_str[i]);
        }
        /* zero any remaining slots */
        for (size_t i = 13; i < needed_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_cap = needed_cap;
        max_computed = 12;
    } else if (needed_cap > cache_cap) {
        /* enlarge existing cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* reallocation failed */
        cache = tmp;
        /* zero new slots */
        for (size_t i = cache_cap; i < needed_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_cap = needed_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 12) {
            /* already initialized */
            continue;
        }
        /* recurrence: a(i) = 10000*a(i-2) + a(i-4) - 10000*a(i-6) */
        seq_t term = (seq_t)10000 * cache[i - 2] + cache[i - 4]
                     - (seq_t)10000 * cache[i - 6];
        cache[i] = term;
    }

    if (n > max_computed) {
        max_computed = n;
    }

    return cache[n];
}
