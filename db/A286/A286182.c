/*
 * A286182  Number of connected induced (non-null) subgraphs of the prism
 *          graph with 2n nodes.
 * https://oeis.org/A286182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 4*a(n-3) + 5*a(n-4) - 2*a(n-5) -
 *            a(n-6) (Conjectured) with
 *            a(1)=3,a(2)=13,a(3)=51,a(4)=167,a(5)=503,a(6)=1441
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset starts at 1, not used otherwise */

    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size (index 0 unused) */
    static idx_t last_computed = 0;
    static int base_set = 0;

    /* ensure capacity for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* out of memory */
        if (tmp != cache) {
            /* zero-initialize newly allocated part */
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
            cache = tmp;
        }
        cap = new_cap;
    }

    /* set the initial 6 terms once */
    if (!base_set) {
        static const seq_t base[6] = {
            (seq_t)3, (seq_t)13, (seq_t)51,
            (seq_t)167, (seq_t)503, (seq_t)1441
        };
        for (idx_t i = 1; i <= 6; ++i) cache[i] = base[i - 1];
        last_computed = 6;
        base_set = 1;
    }

    /* compute up to n if needed */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* recurrence holds for i > 6; for i <= 6 values already set */
            cache[i] = ( 6 * cache[i - 1]
                       - 11 * cache[i - 2]
                       + 4 * cache[i - 3]
                       + 5 * cache[i - 4]
                       - 2 * cache[i - 5]
                       - cache[i - 6] );
        }
        last_computed = n;
    }

    return cache[n];
}
