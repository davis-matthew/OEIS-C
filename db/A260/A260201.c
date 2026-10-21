/*
 * A260201  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00010001.
 * https://oeis.org/A260201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 44, a(2) = 80, a(3) = 207, a(4) = 479, a(5) = 1163, a(6)
 *            = 2864, a(7) = 6895, a(8) = 16690, a(9) = 40598. For n > 9: a(n)
 *            = a(n-1)+a(n-2)+5*a(n-3)+2*a(n-4)+a(n-5)-a(n-7)-a(n-8)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoization buffer */
    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;   /* size in elements, indices 0..cache_sz-1 */
    static size_t cached   = 0;   /* highest index that has a valid value */

    if (n < 1)            /* out of offset range – not used for OEIS */
        return 0;

    /* enlarge buffer if necessary */
    if (cache_sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failure */
        cache  = tmp;
        cache_sz = new_sz;
    }

    /* initial values for indices 1..9 (index 0 unused) */
    static const seq_t init[10] = { (seq_t)0, (seq_t)44, (seq_t)80,
                                    (seq_t)207, (seq_t)479, (seq_t)1163,
                                    (seq_t)2864, (seq_t)6895, (seq_t)16690,
                                    (seq_t)40598 };

    /* fill in any missing initial values that haven't been cached yet */
    size_t limit = (cache_sz < 10 ? cache_sz : 10);
    for (size_t i = 1; i < limit; ++i) {
        if (i > cached) {
            cache[i] = init[i];
            cached   = i;
        }
    }

    /* compute terms beyond the cached range */
    if (cached < n) {
        /* start computing from the smallest needed index
           recurrence is valid for n > 9 */
        idx_t start = (idx_t)cached + 1;
        if (start < 10) start = 10;
        for (idx_t i = start; i <= n; ++i) {
            seq_t term =
                  cache[i-1]
                + cache[i-2]
                + (seq_t)5 * cache[i-3]
                + (seq_t)2 * cache[i-4]
                +        cache[i-5]
                -        cache[i-7]
                -        cache[i-8];
            cache[i] = term;
        }
        cached = n;
    }

    return cache[(size_t)n];
}
