/*
 * A255815  Numbers of words on {0,1,2,3,4,5} having no isolated zeros.
 * https://oeis.org/A255815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 5*a(n-2) + 5*a(n-3) with a(0) = 1, a(1) = 5,
 *            a(2) = 26
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t, idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t capa   = 0;          /* allocated number of elements   */
    static size_t computed = 0;        /* first index that has not yet been computed */

    /* Make sure the array is large enough to hold up to n. */
    size_t need = (size_t)n + 1;
    if (capa < need) {
        size_t newc = capa ? capa << 1 : 1;
        while (newc < need)
            newc <<= 1;
        /* In C++ a cast is required; in C it is implicit. */
        seq_t *tmp = (seq_t *)realloc(cache, newc * sizeof(seq_t));
        if (!tmp)            /* allocation failure – unlikely in the test suite */
            return 0;
        cache = tmp;
        capa  = newc;
    }

    /* Compute all terms up to n which have not yet been computed. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = 5;
        } else if (i == 2) {
            val = 26;
        } else {
            /* a(i) = 6*a(i-1) - 5*a(i-2) + 5*a(i-3) */
            seq_t t = cache[i - 1];
            t *= 6;                      /* 6*a(i-1)   */
            t -= 5 * cache[i - 2];       /* -5*a(i-2)  */
            t += 5 * cache[i - 3];       /* +5*a(i-3)  */
            val = t;
        }
        cache[i] = val;
    }
    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
