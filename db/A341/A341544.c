/*
 * A341544  a(n) = sqrt( Product_{j=1..n} Product_{k=1..4}
 *          (4*sin((2*j-1)*Pi/(2*n))^2 + 4*sin((2*k-1)*Pi/4)^2) ).
 * https://oeis.org/A341544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 76*a(n-2) + 76*a(n-3) - 19*a(n-4) + a(n-5)
 *            with initial values: a(1)=36, a(2)=256, a(3)=2916, a(4)=38416,
 *            a(5)=527076
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cast the index to a native size_t for array access */
    size_t N = (size_t)n;
    if (N == 0)
        return 0; /* n<1 is undefined, return 0 as safe default */

    /* Static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated length of cache   */
    static size_t cur_max  = 0;    /* largest index that has been computed */

    /* First allocation and seed the first five terms */
    if (cache == NULL) {
        cache_len = 6 + 1;         /* index 0 .. 5 (we ignore index 0) */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure, return 0 */
        /* initialize to zero */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = 0;
        /* seed the known values */
        cache[1] = 36;
        cache[2] = 256;
        cache[3] = 2916;
        cache[4] = 38416;
        cache[5] = 527076;
        cur_max = 5;
    }

    /* Ensure the cache is large enough to hold up to N */
    if (N + 1 > cache_len) {
        size_t new_len = N + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* reallocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms up to N using the recurrence */
    if (N > cur_max) {
        for (size_t i = cur_max + 1; i <= N; ++i) {
            /* recurrence derived to avoid negative intermediate:
               a(n) = 19*a(n-1) + 76*a(n-3) + a(n-5)
                      - 76*a(n-2) - 19*a(n-4)          */
            seq_t val = 19 * cache[i - 1];
            val += 76 * cache[i - 3];
            val += cache[i - 5];
            val -= 76 * cache[i - 2];
            val -= 19 * cache[i - 4];
            cache[i] = val;
        }
        cur_max = N;
    }

    return cache[N];
}
