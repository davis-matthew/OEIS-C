/*
 * A299649  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4, 5 or 6 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) +5*a(n-2) +9*a(n-3) -a(n-4) -6*a(n-5) with
 *            initial values a(1)=4, a(2)=32, a(3)=247, a(4)=1924, a(5)=14981
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;

    /* allocate or grow cache as necessary */
    if ((idx_t)cap < n + 1) {
        size_t new_cap = cap > 0 ? cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* initialize first five values when first called */
    if (max_n < 5) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)247;
        cache[4] = (seq_t)1924;
        cache[5] = (seq_t)14981;
        max_n = 5;
    }

    /* compute missing terms */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t t = 7 * cache[i - 1];
        t += 5 * cache[i - 2];
        t += 9 * cache[i - 3];
        t -= cache[i - 4];
        t -= 6 * cache[i - 5];
        cache[i] = t;
    }
    max_n = n > max_n ? n : max_n;
    return cache[n];
}
