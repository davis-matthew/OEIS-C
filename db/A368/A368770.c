/*
 * A368770  a(n) = (n!)^3 * Sum_{k=0..n} k/(k!)^3.
 * https://oeis.org/A368770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n^3 * a(n-1) + n (n>=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Static cache for memoised results */
    static seq_t *cache = NULL;
    static idx_t last = 0;     /* highest index currently cached */
    static size_t capacity = 0;

    /* Initialise cache with a(0) on first call */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (cache == NULL) return (seq_t)0;          /* out of memory */
        cache[0] = (seq_t)0;                          /* a(0) = 0 */
        last = 0;
    }

    /* Grow cache if necessary */
    if (n > last) {
        /* Ensure capacity at least n+1 */
        while (capacity <= (size_t)n) {
            size_t newcap = capacity * 2;
            if (newcap == 0) newcap = 8;
            if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
            if (tmp == NULL) return (seq_t)0;          /* out of memory */
            cache = tmp;
            capacity = newcap;
        }

        /* Compute missing terms up to n using the recurrence */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t si = (seq_t)i;
            seq_t cube = si * si * si;          /* n^3 */
            cache[i] = cube * cache[i - 1] + si;
        }
        last = n;
    }

    return cache[n];
}
