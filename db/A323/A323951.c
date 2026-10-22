/*
 * A323951  Number of ways to split an n-cycle into connected subgraphs, all
 *          having at least three vertices.
 * https://oeis.org/A323951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4) + a(n-5) for n
 *            ≥ 8, with initial values a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1,
 *            a(5)=1, a(6)=4, a(7)=8. The recurrence is derived from the
 *            rational generating function
 *            (x^7-2*x^6+x^3-3*x^2+3*x-1)/((x^3+x-1)*(x-1)^2)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static const seq_t pre[8] = { 1, 0, 0, 1, 1, 1, 4, 8 };

    /* Allocate or grow cache to accommodate index n. */
    if (n + 1 > cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        }
        cache_size = new_size;

        /* Fill newly allocated entries. */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = pre[i];
            } else {
                /* Recurrence a(i) = 3*a(i-1) - 3*a(i-2) + 2*a(i-3) - 2*a(i-4) + a(i-5) */
                seq_t val = 3 * cache[i - 1];
                val -= 3 * cache[i - 2];
                val += 2 * cache[i - 3];
                val -= 2 * cache[i - 4];
                val += cache[i - 5];
                cache[i] = val;
            }
        }
    }
    return cache[n];
}
