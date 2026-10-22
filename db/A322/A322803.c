/*
 * A322803  Number of compositions (ordered partitions) of n into centered
 *          heptagonal numbers (A069099).
 * https://oeis.org/A322803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k≥0, h(k)≤n} a(n−h(k)), where h(k) =
 *            (7*k*(k+1))/2 + 1 (centred heptagonal numbers)
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table of computed values */
    static seq_t *dp      = NULL;  /* dp[i] = a(i) */
    static size_t dp_size = 0;     /* current number of stored values */

    /* initialise on first call */
    if (dp == NULL) {
        dp_size = 1;
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;   /* out‑of‑memory guard */
        dp[0] = (seq_t)1;                 /* a(0) = 1 */
    }

    /* grow the table up to n if necessary */
    if ((size_t)n >= dp_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out‑of‑memory guard */
        dp = tmp;

        /* compute new entries */
        for (size_t i = dp_size; i < new_size; ++i) {
            seq_t sum = (seq_t)0;
            /* iterate over centred heptagonal numbers h(k) <= i */
            for (size_t k = 0;; ++k) {
                size_t h = ((size_t)7 * k * (k + 1)) / 2 + 1;
                if (h > i) break;
                sum += dp[i - h];
            }
            dp[i] = sum;
        }
        dp_size = new_size;
    }

    return dp[(size_t)n];
}
