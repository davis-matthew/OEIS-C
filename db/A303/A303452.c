/*
 * A303452  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8; a(2) = 128; a(3) = 1690; a(4) = 22756; a(5) = 306767;
 *            a(6) = 4136339; a(7) = 55781418; a(8) = 752277525; a(9) =
 *            10145443043; a(10) = 136824986363; a(11) = 1845271319103; a(12)
 *            = 24886004513970; a(13) = 335621796861796; a(14) =
 *            4526318937807297; a(15) = 61043601702103344; a(16) = 8232564...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial values (1..16) */
    static const seq_t init[16] = {
        (seq_t)8,
        (seq_t)128,
        (seq_t)1690,
        (seq_t)22756,
        (seq_t)306767,
        (seq_t)4136339,
        (seq_t)55781418,
        (seq_t)752277525,
        (seq_t)10145443043,
        (seq_t)136824986363,
        (seq_t)1845271319103,
        (seq_t)24886004513970,
        (seq_t)335621796861796,
        (seq_t)4526318937807297,
        (seq_t)61043601702103344,
        (seq_t)823256462690364134
    };

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    if (cap == 0) {  /* first call: allocate and copy the initial 16 terms */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            cache[i] = init[i];
    }

    if ((idx_t)cap >= n)  /* value already computed */
        return cache[n - 1U];

    /* extend cache to length n */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) /* allocation failure: abort */
        abort();
    cache = new_cache;
    /* compute new terms from cap+1 up to n */
    for (idx_t i = cap + 1; i <= n; ++i) {
        /* i is 1-based index of term to compute */
        /* indices are 0-based in `cache` array */
        seq_t val = 14 * cache[i - 2]               /* a(i-1)  */
                 - 60 * cache[i - 4]               /* a(i-3)  */
                 - 434 * cache[i - 5]              /* a(i-4)  */
                 - 284 * cache[i - 6]              /* a(i-5)  */
                 + 588 * cache[i - 7]              /* a(i-6)  */
                 + 2516 * cache[i - 8]             /* a(i-7)  */
                 + 2538 * cache[i - 9]             /* a(i-8)  */
                 + 909 * cache[i -10]              /* a(i-9)  */
                 - 2033 * cache[i -11]             /* a(i-10) */
                 - 2841 * cache[i -12]             /* a(i-11) */
                 - 1886 * cache[i -13]             /* a(i-12) */
                 - 1370 * cache[i -14]             /* a(i-13) */
                 - 740  * cache[i -15]             /* a(i-14) */
                 - 208  * cache[i -16];            /* a(i-15) */
        cache[i - 1] = val;
    }
    cap = n;  /* update cache size */
    return cache[n - 1];
}
