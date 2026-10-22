/*
 * A304368  Numbers n with additive persistence = 3.
 * https://oeis.org/A304368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304368: numbers with additive persistence = 3.  Computed by search. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap    = 0;  /* cache capacity */
    static size_t   count  = 0;  /* number of terms stored */
    static idx_t   next = 199;   /* next integer to test */

    /* Gather terms until we have at least n of them. */
    while (count < n) {
        seq_t val = (seq_t)next;
        next++;

        /* Compute additive persistence. */
        idx_t steps = 0;
        seq_t t = val;
        while (t > (seq_t)9) {
            t = digit_sum(t);
            steps++;
        }

        if (steps == 3) {
            /* Grow cache if needed. */
            if (count >= cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cap = newcap;
            }
            cache[count++] = val;
        }
    }

    return cache[n - 1];
}
