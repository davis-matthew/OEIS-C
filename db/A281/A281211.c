/*
 * A281211  Number of 7 X n 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 32, u64 n <= 77, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A281211
   Empirical recurrence (n>13):
     a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6)
   Initial values:
     a(1)  =   0
     a(2)  = 1308
     a(3)  = 2688
     a(4)  = 3604
     a(5)  = 5712
     a(6)  = 9118
     a(7)  = 14375
     a(8)  = 22700
     a(9)  = 36144
     a(10) = 58168
     a(11) = 94524
     a(12) = 154800
     a(13) = 254959
   Computed iteratively; memoised across calls. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache: array of values a[1..] with a[0] unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */

    /* initial values 1..13, cache_cap >= 14 after first use */
    static const seq_t init_arr[14] = {
        0,    /* dummy 0 index */
        0, 1308, 2688, 3604, 5712,   /* 1..5 */
        9118, 14375, 22700, 36144,  /* 6..9 */
        58168,94524,154800,254959  /*10..13*/
    };

    if (n <= 13) {
        return init_arr[(size_t)n];
    }

    /* ensure cache has room for indices up to n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (n + 1) * 2;   /* grow generously */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed: undefined behaviour in this context */
            /* In contest environment this should never happen, but return 0 as safe guard */
            return 0;
        }
        /* copy old data if any */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* fill cache values up to n if not yet computed */
    size_t i;
    /* initialise initial segment if not already */
    if (cache_cap > 13 && cache[1] == 0) {
        for (i = 1; i <= 13; ++i) {
            cache[i] = init_arr[i];
        }
    }

    for (i = 14; i <= (size_t)n; ++i) {
        /* recurrence: a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) */
        seq_t t1 = cache[i - 1] * 4;
        seq_t t2 = cache[i - 2] * 4;
        seq_t t3 = cache[i - 3] * 2;
        seq_t t4 = cache[i - 4] * 4;
        seq_t t5 = cache[i - 6];
        /* avoid intermediate underflow: compute (t1+t4) - (t2+t3+t5) */
        seq_t sum1 = t1 + t4;
        seq_t sum2 = t2 + t3 + t5;
        cache[i] = sum1 - sum2;
    }

    return cache[(size_t)n];
}
