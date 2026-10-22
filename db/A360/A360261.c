/*
 * A360261  Determinant of the pentadiagonal symmetric n X n Toeplitz Matrix
 *          with a=b=1, c=2.
 * https://oeis.org/A360261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 31, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) + a(n-2) - 2*a(n-3) + 8*a(n-4) + 32*a(n-5);
   Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=-1, a(4)=7 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static idx_t size = 0;
    static seq_t *cache = NULL;

    if (n >= size) {
        idx_t newsize = (n + 1 > 16 ? n + 1 : 16);
        seq_t *new_cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!new_cache) abort();  /* allocation failed */
        cache = new_cache;

        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) cache[i] = 1;
            else if (i == 1) cache[i] = 1;
            else if (i == 2) cache[i] = 0;
            else if (i == 3) cache[i] = -1;
            else if (i == 4) cache[i] = 7;
            else
                cache[i] = -cache[i - 1] + cache[i - 2] - 2 * cache[i - 3]
                          + 8 * cache[i - 4] + 32 * cache[i - 5];
        }
        size = n + 1;
    }

    return cache[n];
}
