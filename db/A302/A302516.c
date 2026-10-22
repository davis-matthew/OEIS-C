/*
 * A302516  Number of 3 X n 0..1 arrays with every element equal to 0, 1, 4 or
 *          5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + 4*a(n-4) for n>7 Initial values:
 *            a(1)=4, a(2)=4, a(3)=11, a(4)=15, a(5)=21, a(6)=52, a(7)=118
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;
static bool seq_cache_initialized = false;
static idx_t seq_cache_last = 7; /* highest index computed */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence offset starts at 1 */

    /* Ensure the cache is large enough (at least 8 for the base cases) */
    size_t needed = (size_t)n + 1;
    if (needed < 8) needed = 8;
    if (needed > seq_cache_size) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        seq_cache = tmp;
        seq_cache_size = needed;
    }

    /* Initialise the first eight entries once */
    if (!seq_cache_initialized) {
        seq_cache[0] = 0;
        seq_cache[1] = (seq_t)4;
        seq_cache[2] = (seq_t)4;
        seq_cache[3] = (seq_t)11;
        seq_cache[4] = (seq_t)15;
        seq_cache[5] = (seq_t)21;
        seq_cache[6] = (seq_t)52;
        seq_cache[7] = (seq_t)118;
        seq_cache_initialized = true;
        seq_cache_last = 7;
    }

    if (n <= seq_cache_last) return seq_cache[n];

    for (idx_t i = seq_cache_last + 1; i <= n; ++i) {
        seq_cache[i] = seq_cache[i-1]
                     + (seq_cache[i-3] * (seq_t)2)
                     + (seq_cache[i-4] * (seq_t)4);
    }
    seq_cache_last = n;
    return seq_cache[n];
}
