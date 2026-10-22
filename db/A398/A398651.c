/*
 * A398651  Lower (1, 1/2)-midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6); Initial values: a(0)=0, a(1)=1, a(2)=5, a(3)=13,
 *            a(4)=25, a(5)=42
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (n + 1 > size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        for (idx_t i = size; i < new_size; ++i) {
            if (i == 0)                cache[i] = 0;
            else if (i == 1)            cache[i] = 1;
            else if (i == 2)            cache[i] = 5;
            else if (i == 3)            cache[i] = 13;
            else if (i == 4)            cache[i] = 25;
            else if (i == 5)            cache[i] = 42;
            else
                cache[i] = cache[i-1]*4
                         - cache[i-2]*7
                         + cache[i-3]*8
                         - cache[i-4]*7
                         + cache[i-5]*4
                         - cache[i-6];
        }
        size = new_size;
    }
    return cache[n];
}
