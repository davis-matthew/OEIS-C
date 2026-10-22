/*
 * A345793  Numbers that are the sum of nine cubes in exactly one way.
 * https://oeis.org/A345793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that are the sum of nine positive cubes in exactly one
 *            way
 * Range:     u8 n <= 120, u16 n <= 176, u32 n <= 176, u64 n <= 176,
 *            u128 n <= 176, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the nth term of A345793.  The function is memoised, so
   future calls, even out of order, reuse the pre‑computed table. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0 */

    /* ---------- One‑time initialisation --------------------------------- */
    static seq_t *unique = NULL;   /* vector of unique sums (values)   */
    static idx_t unique_len = 0;   /* number of stored entries         */
    static bool init = false;      /* flag                                         */

    /* Pre‑compute up to some maximum sum.  The limit is chosen to
       comfortably cover at least 10000 terms; adjust if necessary.          */
    const idx_t MAX_SUM = 200000;  /* sufficient for the first ~10000 terms   */

    if (!init) {
        /* Interval [1 .. MAX_SUM] will be searched for sums with
           exactly one representation as a sum of nine positive cubes. */

        /* dp[k][s] – number of ways to write s as a sum of k positive
           cubes, ignoring order, capped at 2.  We store the table as a
           flat array to minimise overhead.                           */
        const size_t KMAX = 9;
        const size_t SZ   = (KMAX + 1) * (MAX_SUM + 1);
        uint8_t *dp = (uint8_t *)malloc(SZ);
        if (!dp) return 0;          /* allocation failed – give up */

        /* initialise all entries to 0, then seed dp[0][0] = 1 */
        for (size_t i = 0; i < SZ; ++i) dp[i] = 0;
        dp[0] = 1;                   /* dp[0][0] = 1 */

        /* Determine all positive cube values that are <= MAX_SUM */
        idx_t vmax = 1;
        while ((vmax + 1) * (vmax + 1) * (vmax + 1) <= MAX_SUM) ++vmax;

        /* Dynamic programming: for each cube value, update counts for
           1..9 cubes.  The update order (inner loop over s) guarantees
           that each combination of cubes is counted exactly once
           (unordered).  Values are capped at 2 so we need only know
           whether the count is 0, 1 or >1.                          */
        for (idx_t v = 1; v <= vmax; ++v) {
            idx_t cube = v * v * v;
            for (size_t k = 1; k <= KMAX; ++k) {
                uint8_t *dst = dp + k * (MAX_SUM + 1);
                uint8_t *src = dp + (k - 1) * (MAX_SUM + 1);
                for (idx_t s = cube; s <= MAX_SUM; ++s) {
                    uint8_t add = src[s - cube];
                    if (add) {
                        uint8_t new_val = dst[s] + add;
                        dst[s] = (new_val > 2) ? 2 : new_val;
                    }
                }
            }
        }

        /* ------------------ Extract the unique sums ---------------------- */
        size_t cap = 1024;
        unique = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!unique) { free(dp); return 0; }
        unique_len = 0;

        for (idx_t s = 1; s <= MAX_SUM; ++s) {
            /* We want sums that have exactly ONE representation as a sum
               of nine positive cubes. */
            if (dp[9 * (MAX_SUM + 1) + s] == 1) {
                if (unique_len >= cap) {
                    cap <<= 1;                         /* grow vector */
                    seq_t *tmp = (seq_t *)realloc(unique, cap * sizeof(seq_t));
                    if (!tmp) { free(dp); return 0; }
                    unique = tmp;
                }
                unique[unique_len++] = (seq_t)s;
            }
        }
        free(dp);
        init = true;
    }

    /* ---------- Answer the query --------------------------------------- */
    if (n - 1 < unique_len) return unique[n - 1];
    return 0;          /* undefined beyond the pre‑computed range */
}
