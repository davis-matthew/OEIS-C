/*
 * A266976  Decimal representation of the n-th iteration of the "Rule 78"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) - 32*a(n-3) for n >= 4 * Initial
 *            values: a(0)=1, a(1)=6, a(2)=28, a(3)=104
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* lazy initialization of cache with first four terms */
    if (cache_len == 0) {
        cache_len = 4; /* indices 0..3 */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)28;
        cache[3] = (seq_t)104;
    }

    /* extend cache up to requested index n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        while (cache_len <= n) {
            /* a(k) = 2*a(k-1) + 16*a(k-2) - 32*a(k-3)  */
            seq_t term = cache[cache_len-1] * 2
                       + cache[cache_len-2] * 16
                       - cache[cache_len-3] * 32;
            cache[cache_len] = term;
            cache_len++;
        }
    }

    return cache[n];
}
