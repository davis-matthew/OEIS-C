/*
 * A341582  Number of simple moves of the smallest disk in the solution to the
 *          Towers of Hanoi exchanging disks puzzle with 3 pegs and n disks.
 * https://oeis.org/A341582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341582: a(0)=0, a(1)=1, a(2)=2, a(3)=4,
 * a(n)=a(n-1)+a(n-2)+2*a(n-4) for n>=4   (linear recurrence) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values.
       cache[i] holds a(i) for 0 <= i < cache_size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries stored */

    /* Need to compute up to n inclusive.  Increase cache if necessary. */
    if ((idx_t)n + 1 > cache_size) {
        /* New size requested */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {   /* allocation failed – undefined behaviour for this problem */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)2;
            } else if (i == 3) {
                cache[i] = (seq_t)4;
            } else {
                /* recurrence: a(i) = a(i-1) + a(i-2) + 2*a(i-4) */
                cache[i] = cache[i - 1] + cache[i - 2] + (cache[i - 4] << 1);
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
