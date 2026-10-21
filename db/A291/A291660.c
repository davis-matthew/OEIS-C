/*
 * A291660  a(n) = 2*a(n-1) - a(n-2) + a(n-4) for n>3, a(0)=2, a(1)=3, a(2)=5,
 *          a(3)=7, a sequence related to Lucas numbers.
 * https://oeis.org/A291660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-4)
   Initial values: a(0)=2, a(1)=3, a(2)=5, a(3)=7 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* size of allocated array */
    static idx_t last = 3;  /* highest index computed so far */

    /* Allocate and initialise cache on first call */
    if (cache == NULL) {
        cap = 4;                           /* need indices 0..3 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();               /* memory error */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
    }

    /* Expand cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0) msg: val = (seq_t)2;
        else if (i == 1) val = (seq_t)3;
        else if (i == 2) val = (seq_t)5;
        else if (i == 3) val = (seq_t)7;
        else
            val = (seq_t)2 * cache[i - 1] - cache[i - 2] + cache[i - 4];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
