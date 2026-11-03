/*
 * A268128  a(n) = (A000123(n) - A001316)/2.
 * https://oeis.org/A268128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000123(n) - A001316(n))/2
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000123(n) = b(n) with b(0)=1, b(n)=b(n-1)+b(n/2) */
/* and A001316(n) = c(n) = 2^popcount(n) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free(seq_t *p) { mem_free(p); }

seq_t a(idx_t n)
{
    /* memoisation array for A000123 */
    static seq_t *b   = NULL;
    static size_t max = 0;     /* number of computed entries 0..max-1 */

    /* Ensure we have computed up to n */
    if (max <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (!tmp)
            return (seq_t)0;   /* fallback on allocation failure */

        /* copy old values, if any */
        for (size_t i = 0; i < max; ++i)
            tmp[i] = b[i];

        /* initialise first element on first allocation */
        if (max == 0)
            tmp[0] = (seq_t)1;

        /* compute the remaining b(i) values */
        for (size_t i = max; i < new_cap; ++i) {
            if (i == 0) continue;
            tmp[i] = tmp[i - 1] + tmp[i / 2];
        }

        /* free old array and update */
        if (b) seq_free(b);
        b   = tmp;
        max = new_cap;
    }

    /* c(n) = 2^popcount(n) */
    unsigned long long pc = __builtin_popcountll((unsigned long long)n);
    seq_t c = (seq_t)1;
    for (size_t i = 0; i < pc; ++i)
        c <<= 1;

    /* a(n) = (b(n) - c) / 2 */
    return (b[(size_t)n] - c) / 2;
}
