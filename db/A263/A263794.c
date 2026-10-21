/*
 * A263794  Number of (n+1) X (3+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits, and rows and columns lexicographically
 *          nonincreasing.
 * https://oeis.org/A263794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 144, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263794
 * Recurrence: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) - 3*a(n-4) + 3*a(n-5) + a(n-6) - a(n-7)
 * Initial values (a(1)..a(7)): 3,3,7,7,14,14,25
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache holds terms a(1), a(2), ..., a(size)
     * cache[0] == a(1), cache[1] == a(2), etc.  */
    static seq_t *cache = NULL;     /* pointer to cached terms */
    static idx_t size = 0;          /* number of terms currently stored */

    if (n == (idx_t)0) {            /* offset starts at 1; undefined for 0 */
        return (seq_t)0;
    }

    /* If requested term not yet computed, extend cache.  */
    if (n > size) {
        idx_t new_size = n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!cache) abort();
        } else if (new_size > size) {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!cache) abort();
        }

        /* fill new entries */
        for (idx_t i = size + 1; i <= n; ++i) {
            if (i <= 7) {                    /* base values */
                static const seq_t base[7] = {3,3,7,7,14,14,25};
                cache[i - 1] = base[i - 1];
            } else {                         /* recurrence for i >= 8 */
                seq_t a1 = cache[i - 2];     /* a(i-1)  */
                seq_t a2 = cache[i - 3];     /* a(i-2)  */
                seq_t a3 = cache[i - 4];     /* a(i-3)  */
                seq_t a4 = cache[i - 5];     /* a(i-4)  */
                seq_t a5 = cache[i - 6];     /* a(i-5)  */
                seq_t a6 = cache[i - 7];     /* a(i-6)  */
                seq_t a7 = cache[i - 8];     /* a(i-7)  */

                cache[i - 1] = a1 + 3*a2 - 3*a3 - 3*a4 + 3*a5 + a6 - a7;
            }
        }
        size = new_size;
    }
    return cache[n - 1];
}
