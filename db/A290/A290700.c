/*
 * A290700  Number of minimal edge covers in the n-prism graph.
 * https://oeis.org/A290700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 6*a(n-3) + 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) - 2*a(n-7) - a(n-8) + a(n-9) Initial values (for n ≥
 *            1): a(1)=1, a(2)=5, a(3)=25, a(4)=49, a(5)=141, a(6)=389,
 *            a(7)=1009, a(8)=2761, a(9)=7441
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* we treat the sequence as 1‑based; cache[0] is unused */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* highest n whose value is known */
    static idx_t max_capacity = 0;   /* current allocated size (indices count) */

    if (n == 0) return 0;   /* safety, not part of the sequence */

    /* ensure a buffer large enough for indices up to n */
    if (cache == NULL) {
        /* first allocation: at least up to n or to 9, whichever is larger */
        idx_t init_cap = (n > 9 ? n : 9) + 1;   /* +1 because we use index 0 as dummy */
        cache = (seq_t *)malloc((size_t)init_cap * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failure */
        max_capacity = init_cap;
        /* initialise base cases */
        static const seq_t base[9] = {
            (seq_t)1, (seq_t)5, (seq_t)25,
            (seq_t)49, (seq_t)141, (seq_t)389,
            (seq_t)1009, (seq_t)2761, (seq_t)7441
        };
        cache[0] = 0;   /* unused */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = base[i-1];
        max_computed = 9;
    } else if (n + 1 > max_capacity) {      /* need to grow the buffer */
        idx_t new_cap = (n > 9 ? n : 9) + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* realloc failed – keep old buffer */
        cache = tmp;
        max_capacity = new_cap;
    }

    /* return immediately if value already computed */
    if (n <= max_computed)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = cache[i-1];
        v += (seq_t)2 * cache[i-2];
        v += (seq_t)6 * cache[i-3];
        v += (seq_t)2 * cache[i-4];
        v += (seq_t)2 * cache[i-5];
        v -= (seq_t)2 * cache[i-6];
        v -= (seq_t)2 * cache[i-7];
        v -=  cache[i-8];
        v +=  cache[i-9];
        cache[i] = v;
    }
    max_computed = n;
    return cache[n];
}
