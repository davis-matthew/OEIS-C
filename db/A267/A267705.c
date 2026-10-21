/*
 * A267705  Binary representation of the n-th iteration of the "Rule 205"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=10101, a(3)=1101011; For n>3:
 *            a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) (coefficients are
 *            decimal)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cache_size = 0;
    static idx_t   max_computed = 3;          /* highest index already stored */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 4;                       /* indices 0..3 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)10101;
        cache[3] = (seq_t)1101011;
        max_computed = 3;
    }

    /* grow the cache if needed */
    size_t required = (size_t)n + 1;
    if (required > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < required) new_size = required;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* return already known value */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (size_t i = (size_t)max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)111  * cache[i - 1]
                 - (seq_t)1110 * cache[i - 2]
                 + (seq_t)1000 * cache[i - 3];
    }

    max_computed = (idx_t)n;
    return cache[(size_t)n];
}
