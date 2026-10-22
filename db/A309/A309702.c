/*
 * A309702  G.f. A(x) satisfies: A(x) = A(x^2) / (1 - x - x^2 - x^3).
 * https://oeis.org/A309702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0, a(n) = a(n-1)+a(n-2)+a(n-3)+(n even ? a(n/2)
 *            : 0) for n > 0
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values, indexed by n   */
    static size_t cap   = 0;      /* capacity of cache (elements)   */

    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* abort on allocation failure */
        cache = tmp;

        for (idx_t i = cap; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 1;                       /* a(0) = 1 */
            } else {
                val = 0;
                if (i >= 1) val += cache[i - 1];
                if (i >= 2) val += cache[i - 2];
                if (i >= 3) val += cache[i - 3];
                if ((i & 1) == 0) val += cache[i / 2];
            }
            cache[i] = val;
        }
        cap = new_cap;
    }
    return cache[n];
}
