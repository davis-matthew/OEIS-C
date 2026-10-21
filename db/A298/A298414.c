/*
 * A298414  a(n) = 2*a(n-1) - a(n-2) + a([n/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a([n/2]) (a(0)=1, a(1)=1, a(2)=1)
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 167, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of OEIS A298414.
   The function is memoised over a static buffer that grows as needed.
   It works for any build of seq_t (built‑in unsigned type or a helper
   big‑int class) and for any order of calls. */
seq_t a(idx_t n)
{
    /* static per‑process cache */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated capacity in elements */
    static size_t sz      = 0;   /* number of values stored (sz == 0 -> empty) */

    /* Ensure we have space for index n (inclusive). */
    if ((size_t)n >= cap) {
        size_t new_cap = cap > 0 ? cap << 1 : 4;   /* double until large enough */
        while (new_cap <= (size_t)n) new_cap <<= 1;

        /* realloc may fail; keep old buffer if it does. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cap   = new_cap;
        }
    }

    /* Compute any missing terms up to n, inclusive. */
    if (sz <= (size_t)n) {
        for (size_t i = sz; i <= (size_t)n; ++i) {
            if (i < 3) {
                cache[i] = (seq_t)1;                /* a(0)=a(1)=a(2)=1 */
            } else {
                seq_t v1 = cache[i - 1];
                seq_t v2 = cache[i - 2];
                seq_t v3 = cache[i >> 1];           /* floor(i/2) */
                cache[i] = (v1 + v1) - v2 + v3;     /* 2*v1 - v2 + v3 */
            }
        }
        sz = (size_t)n + 1;
    }

    return cache[n];
}
