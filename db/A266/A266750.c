/*
 * A266750  G.f.:
 *          1/((1-t^10)*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^9)*(1-t^11)*(1-t^13)*(1-t^15)*(1-t^17)*(1-t^19)).
 * https://oeis.org/A266750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with part sizes in the set
 *            {1,3,5,7,9,10,11,13,15,17,19}. This is equivalent to the
 *            coefficient of t^n in
 *            1/((1-t^10)*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^9)*(1-t^11)*(1-t^13)*(1-t^15)*(1-t^17)*(1-t^19))
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *dp = NULL;
    static idx_t cache_size = 0;   /* number of elements stored (dp[0 .. cache_size-1]) */

    if (n >= cache_size) {
        /* Allocate (or grow) the cache to accommodate n */
        idx_t new_size = n + 1;          /* we need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) {                      /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        dp = tmp;

        /* Zero all entries up to the new size; they will be recomputed below */
        for (idx_t i = 0; i < new_size; ++i) {
            dp[i] = (seq_t)0;
        }

        /* Compute partition counts up to n using the allowed part sizes */
        dp[0] = (seq_t)1;                /* empty partition */

        /* The allowed part sizes */
        const idx_t parts[] = {1,3,5,7,9,10,11,13,15,17,19};
        const size_t parts_cnt = sizeof(parts)/sizeof(parts[0]);

        for (size_t p = 0; p < parts_cnt; ++p) {
            idx_t k = parts[p];
            for (idx_t i = k; i <= n; ++i) {
                dp[i] += dp[i - k];
            }
        }

        cache_size = new_size;
    }

    return dp[n];
}
