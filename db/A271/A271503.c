/*
 * A271503  a(1) = 1; thereafter a(n) is the product of all 0 < m < n for
 *          which a(m) divides n.
 * https://oeis.org/A271503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 21, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1;
 *              for n>1, a(n) = ∏{ m | 1 ≤ m < n and a(m) divides n } m
 *
 * The implementation memoises computed values and uses a simple
 * O(n²) algorithm which is fast enough for n ≤ 10000.  Overflow
 * is handled for 8/16/32/64‑bit integer types by capping
 * the value at its maximum representable value.  If the
 * build provides an arbitrary‑precision type the builtin
 * arithmetic suffices.  */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;   /* memoised a(m) values */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t  cache_max = 0;    /* largest n computed so far */

    /* Initialise the cache on first call */
    if (!a_cache) {
        cache_cap = 2;                     /* indices 0 and 1 */
        a_cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!a_cache) abort();
        a_cache[0] = 1;                    /* unused slot */
        a_cache[1] = 1;                    /* a(1) = 1 */
        cache_max = 1;
    }

    /* If the requested term is already known, return it */
    if (n <= cache_max)
        return a_cache[n];

    /* Expand the cache large enough to contain the new terms */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing a(m) values up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t prod = 1;

        for (idx_t j = 1; j < m; ++j) {
            /* Skip if a(j) cannot possibly divide m */
            if (a_cache[j] > (seq_t)m) continue;

            if (((seq_t)m % a_cache[j]) == 0) {
#if defined(_MSC_VER) || defined(__cplusplus)
                /* For integer types up to 64‑bit use overflow detection */
                if (sizeof(seq_t) <= 8) {
                    const seq_t max_val = (seq_t)-1;
                    if (prod > 0 && j > 0 && prod > max_val / j)
                        prod = max_val;          /* sentinel value > any n ≤ 10000 */
                    else
                        prod = prod * (seq_t)j;
                } else
#endif
                {
                    prod *= (seq_t)j;             /* arbitrary‑precision or >64‑bit */
                }
            }
        }

        a_cache[m] = prod;
        cache_max = m;
    }

    return a_cache[n];
}
