/*
 * A248988  Number of length 1+4 0..n arrays with every five consecutive terms
 *          having two times the sum of some three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A248988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by a(1)=2, a(2)=53, …, a(12)=45353 and for n≥13:
 *            a(n)=3*a(n-2)+2*a(n-3)−3*a(n-4)−6*a(n-5)+6*a(n-7)+3*a(n-8)−2*a(n-9)−3*a(n-10)+a(n-12)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 205, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   base_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/base_val.h"

seq_t a(idx_t n)
{
    if (n <= 12) return base_val(n);

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;    /* largest index already filled in cache */

    /* Ensure cache large enough for indices 0..n (0 unused) */
    size_t need = (size_t)n + 1;
    if (cap < need) {
        size_t newcap = (cap ? cap : 32);
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap) * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Initialize base terms if not yet done */
    if (computed < 12) {
        for (idx_t i = 1; i <= 12; ++i) {
            cache[i] = base_val(i);
        }
        computed = 12;
    }

    /* Compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val =
            (seq_t)3 * cache[i - 2] + (seq_t)2 * cache[i - 3]
            - (seq_t)3 * cache[i - 4] - (seq_t)6 * cache[i - 5]
            + (seq_t)6 * cache[i - 7] + (seq_t)3 * cache[i - 8]
            - (seq_t)2 * cache[i - 9] - (seq_t)3 * cache[i -10]
            + cache[i -12];
        cache[i] = val;
    }
    computed = n;

    return cache[n];
}
