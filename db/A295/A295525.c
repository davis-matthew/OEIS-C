/*
 * A295525  Number of n X 2 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 2 or 4 1s.
 * https://oeis.org/A295525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 5*a(n-3) - a(n-5) - a(n-6) for n ≥
 *            7, initial values: a(1)=2, a(2)=10, a(3)=29, a(4)=87, a(5)=280,
 *            a(6)=876
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds values computed so far, indexed from 0..max_n.
       cache[0] is never used. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of allocated slots (size) */
    static const seq_t init_vals[] = { (seq_t)2, (seq_t)10, (seq_t)29,
                                       (seq_t)87, (seq_t)280, (seq_t)876 };
    const idx_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);

    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, safe fallback */

    /* enlarge cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;             /* allocation failed - unlikely */
        cache = tmp;
        /* fill new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= init_len) {
                cache[i] = init_vals[i - 1];
            } else {
                seq_t sum = (seq_t)2 * cache[i - 1] +
                            (seq_t)2 * cache[i - 2] +
                            (seq_t)5 * cache[i - 3];
                cache[i] = sum - cache[i - 5] - cache[i - 6];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
