/*
 * A257767  Positive integers whose square is the sum of 33 consecutive
 *          squares.
 * https://oeis.org/A257767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 27, u64 n <= 62, u128 n <= 132,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257767: Positive integers whose square is the sum of 33 consecutive squares.
   Recurrence: a(n) = 46*a(n-6) - a(n-12)  for n > 12
   Initial values:
     a(1) = 143
     a(2) = 253
     a(3) = 440
     a(4) = 1133
     a(5) = 1397
     a(6) = 3608
     a(7) = 6325
     a(8) = 11495
     a(9) = 20152
     a(10)= 52063
     a(11)= 64207
     a(12)=165880
*/

#define _DEFAULT_SOURCE
#include <stddef.h>
#include <stdbool.h>

/* Helpers for memory allocation of seq_t arrays */
#include "../utils/seq_alloc.h"       /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"    /* seq_realloc(seq_t*,size_t,size_t) */

static seq_t *cache = NULL;     /* cache[0] unused, cache[1] = a(1) */
static size_t cap = 0;          /* current capacity of cache */
static idx_t  max_n = 0;        /* maximum index computed so far */

/* Pre‑computed initial terms */
static const seq_t init[13] = {
    (seq_t)0,   /* unused: a(0) */
    (seq_t)143,
    (seq_t)253,
    (seq_t)440,
    (seq_t)1133,
    (seq_t)1397,
    (seq_t)3608,
    (seq_t)6325,
    (seq_t)11495,
    (seq_t)20152,
    (seq_t)52063,
    (seq_t)64207,
    (seq_t)165880
};

seq_t a(idx_t n) {
    /* 1‑based indexing; n>=1 due to problem statement */
    if (n <= 12) {
        /* Return one of the pre‑computed initial terms */
        return init[(size_t)n];
    }

    /* Initialise cache on first use */
    if (cache == NULL) {
        /* Start with a capacity that holds at least the first 12 terms */
        cap = 128;
        cache = seq_alloc(cap);
        /* Copy initial values into cache */
        for (size_t i = 1; i <= 12; ++i) {
            cache[i] = init[i];
        }
        max_n = 12;
    }

    /* Expand cache if needed */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) {
            new_cap = new_cap * 2;
        }
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* Compute terms up to index n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = 46*a(i-6) - a(i-12) */
        seq_t term = (seq_t)46 * cache[i - 6] - cache[i - 12];
        cache[i] = term;
    }
    max_n = n;

    return cache[n];
}
