/*
 * A320429  The length of the shortest prefix of the Thue-Morse word
 *          decomposable to not less than n palindromes.
 * https://oeis.org/A320429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n-3) - 6 for n >= 4 Initial values: a(1)=1, a(2)=2,
 *            a(3)=6 (n is 1‑based)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for offset 1 – safety */
        return (seq_t)0;

    /* cache holds terms 1..current_size (index 0 corresponds to a(1)) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* grow cache if needed */
    if (n > cache_size) {
        size_t new_size = (size_t)n;                /* n fits in size_t for our limits */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        /* initialise newly allocated area */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 3) {                               /* a(1), a(2), a(3) */
                switch (i) {
                    case 0: cache[i] = (seq_t)1; break;
                    case 1: cache[i] = (seq_t)2; break;
                    case 2: cache[i] = (seq_t)6; break;
                }
            } else {
                /* recurrence: a(n+3)=16*a(n)-6  <=>  a(i+1)=16*a(i-2)-6 */
                cache[i] = (seq_t)16 * cache[i - 3] - (seq_t)6;
            }
        }
        cache_size = new_size;
    }

    return cache[n - 1];      /* n-1 is the 0‑based index */
}
