/*
 * A256429  a(n) is the total number of pentagrams in a variant of pentagram
 *          fractal after n iterations.
 * https://oeis.org/A256429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)-4*a(n-2)+7*a(n-3)-5*a(n-4) for n>3, a(0)=1,
 *            a(1)=6, a(2)=11, a(3)=16
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    const seq_t init[4] = { (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)16 };

    if (n < 4)
        return init[n];

    /* Initialise cache if this is the first call. */
    if (cap == 0) {
        cap   = 8;
        cache = alloc_seq(cap);
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        len = 4;
    }

    /* Grow the allocation if required. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = alloc_seq(new_cap);
        for (size_t i = 0; i < len; ++i)
            tmp[i] = cache[i];
        /* old array can stay as the program has static lifetime. */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i-1];  t1 = 3 * t1;   /* 3*a[i-1] */
        seq_t t2 = cache[i-2];  t2 = 4 * t2;   /* 4*a[i-2] */
        seq_t t3 = cache[i-3];  t3 = 7 * t3;   /* 7*a[i-3] */
        seq_t t4 = cache[i-4];  t4 = 5 * t4;   /* 5*a[i-4] */
        cache[i] = (t1 + t3) - (t2 + t4);
    }

    len = (size_t)n + 1;
    return cache[n];
}
