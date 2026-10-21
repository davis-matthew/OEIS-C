/*
 * A290893  p-INVERT of the positive integers, where p(S) = 1 - S^5.
 * https://oeis.org/A290893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A290893: p-INVERT of the positive integers with p(S)=1-S^5
 * 
 * Recurrence:
 *   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) + 253*a(n-5)
 *          - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) - a(n-10)
 * Initial terms (n = 0..9):
 *   0, 0, 0, 0, 1, 10, 55, 220, 715, 2003
 *
 * The implementation caches computed values so that calls in any order are
 * answered in O(max(0,n)) time.  The code uses only integer arithmetic, so
 * it compiles both as C11 and C++17, handling either fixed‑width integer
 * types or an arbitrary‑precision integer class for seq_t.
 */

#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Static cache and its bookkeeping */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;   /* array capacity */
    static idx_t  size    = 0;   /* highest computed index + 1 */

    /* Base capacity */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);  /* allocation error */
    }

    /* Grow the array if necessary */
    if (n >= cap) {
        idx_t old_cap = cap;
        while (n >= cap) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cap * sizeof(seq_t));
        if (!tmp) { free(cache); exit(EXIT_FAILURE); }
        cache = tmp;
        /* initialise new elements to zero */
        for (idx_t i = old_cap; i < cap; ++i) cache[i] = 0;
    }

    /* Known initial values (protect against eager default zeroing) */
    static const seq_t init_vals[10] = {
        0, 0, 0, 0, 1, 10, 55, 220, 715, 2003
    };
    if (size < 10) {
        idx_t upto = 10;
        if (upto > n + 1) upto = n + 1;
        for (idx_t i = size; i < upto; ++i) cache[i] = init_vals[i];
        size = upto;
    }

    /* Return if already computed */
    if (n < size) return cache[n];

    /* Compute from the first missing index up to n */
    for (idx_t i = size; i <= n; ++i) {
        /* Recurrence needs the last 10 values.  For i>=10 this is safe. */
        seq_t v = cache[i-1];
        v = v * (seq_t)10;
        v = v - cache[i-2] * (seq_t)45;
        v = v + cache[i-3] * (seq_t)120;
        v = v - cache[i-4] * (seq_t)210;
        v = v + cache[i-5] * (seq_t)253;
        v = v - cache[i-6] * (seq_t)210;
        v = v + cache[i-7] * (seq_t)120;
        v = v - cache[i-8] * (seq_t)45;
        v = v + cache[i-9] * (seq_t)10;
        v = v - cache[i-10];
        cache[i] = v;
    }
    size = n + 1;
    return cache[n];
}
