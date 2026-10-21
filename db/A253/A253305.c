/*
 * A253305  Numbers n such that the octagonal number O(n) is equal to the sum
 *          of the heptagonal numbers H(m) and H(m+1) for some m.
 * https://oeis.org/A253305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+62*a(n-2)-62*a(n-3)-a(n-4)+a(n-5), a(1) = 2, a(2)
 *            = 29, a(3) = 100, a(4) = 1777, a(5) = 6178
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for the offset, return 0 */
        return (seq_t)0;

    static seq_t *cache = NULL;      /* 1‑based cache */
    static idx_t cached = 0;         /* highest index computed so far */

    if (cache == NULL) {
        /* allocate space for indices 0..5 (index 0 unused) */
        cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)(6));
        if (cache == NULL)         /* allocation failed, give up */
            return (seq_t)0;
    }

    if (cached < n) {
        size_t new_size = (size_t)(n + 1); /* indices 0..n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp != NULL)
            cache = tmp;          /* use the new buffer if realloc succeeded */

        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)2;
            else if (i == 2)
                cache[i] = (seq_t)29;
            else if (i == 3)
                cache[i] = (seq_t)100;
            else if (i == 4)
                cache[i] = (seq_t)1777;
            else if (i == 5)
                cache[i] = (seq_t)6178;
            else {
                seq_t t1 = cache[i-1];
                seq_t t2 = cache[i-2];
                seq_t t3 = cache[i-3];
                seq_t t4 = cache[i-4];
                seq_t t5 = cache[i-5];
                cache[i] = t1 + ((seq_t)62 * t2)
                         - ((seq_t)62 * t3)
                         - t4 + t5;
            }
        }
        cached = n;
    }

    return cache[n];
}
