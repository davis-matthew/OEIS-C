/*
 * A341792  Number of partitions of n into 9 nonzero tetrahedral numbers.
 * https://oeis.org/A341792
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 nonzero tetrahedral
 *            numbers (k(k+1)(k+2)/6). The function uses a DP that counts
 *            combinations of tetrahedral numbers with exactly nine parts
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(idx_t n) for n >= 9.
   For n < 9 the function returns 0, which is irrelevant for the
   OEIS offset. */
seq_t a(idx_t n)
{
    if (n < 9)
        return 0;

    /* Discover all tetrahedral numbers <= n. */
    size_t cap = 8;
    idx_t *tetra = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!tetra)
        return 0; /* allocation failure – return 0 */

    size_t tet_cnt = 0;
    for (idx_t k = 1;; ++k) {
        seq_t tval = (seq_t)k * (k + 1) * (k + 2) / 6;
        if (tval > n)
            break;
        if (tet_cnt >= cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(tetra, cap * sizeof(idx_t));
            if (!tmp) {
                free(tetra);
                return 0;
            }
            tetra = tmp;
        }
        tetra[tet_cnt++] = (idx_t)tval;
    }

    /* DP table: dp[j][s] = number of ways to write s as a sum
       of j tetrahedral numbers, using the considered set of
       tetrahedral numbers so far.  We flatten the table. */
    const size_t parts = 10;                  /* indices 0..9 */
    const size_t cols  = (size_t)n + 1;
    const size_t dp_size = parts * cols;

    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) {
        free(tetra);
        return 0;
    }
    dp[0 * cols + 0] = 1;  /* empty sum with 0 parts */

    /* Unbounded coin change counting order‑free partitions. */
    for (size_t ti = 0; ti < tet_cnt; ++ti) {
        idx_t tval = tetra[ti];
        for (size_t j = 1; j < parts; ++j) {          /* 1..9 parts */
            for (idx_t s = tval; s <= n; ++s) {
                size_t idx = j * cols + (size_t)s;
                size_t prev = (j - 1) * cols + (size_t)(s - tval);
                dp[idx] += dp[prev];
            }
        }
    }

    seq_t ans = dp[9 * cols + (size_t)n];
    free(dp);
    free(tetra);
    return ans;
}
