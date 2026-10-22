/*
 * A381025  Expansion of e.g.f. -log(1-x)^3 * exp(x) / (6 * (1-x)).
 * https://oeis.org/A381025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * |Stirling1(k+1,4)|
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Compute unsigned Stirling numbers of the first kind up to k=4
    using the recurrence   c(n,k) = c(n-1,k-1) + (n-1)*c(n-1,k)  */
static seq_t *t_cache = NULL;          /* |s(m,4)| for m = 0 .. */
static idx_t cache_len = 0;            /* size of t_cache   */

seq_t a(idx_t n)
{
    /* Prepare array of |s(m,4)| up to m = n+1 */
    idx_t need = n + 2;               /* allocate indices 0 .. n+1 */
    if (need > cache_len) {
        /* Reallocate and recompute */
        if (t_cache) free(t_cache);
        t_cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!t_cache) return 0;       /* allocation failure – unlikely */

        /* recurrence tables for c(n,k)   k = 0 .. 4   */
        seq_t prev[5] = {0}, cur[5];
        prev[0] = 1;                  /* c(0,0)=1 */

        t_cache[0] = 0;               /* |s(0,4)| = 0 */

        for (idx_t m = 1; m <= n + 1; ++m) {
            cur[0] = 0;
            for (int k = 1; k <= 4; ++k) {
                seq_t part1 = prev[k - 1];
                seq_t part2 = (seq_t)(m - 1) * prev[k];
                cur[k] = part1 + part2;    /* unsigned Stirling number */
            }
            t_cache[m] = cur[4];          /* |s(m,4)| */
            /* copy cur to prev for next iteration */
            for (int k = 0; k < 5; ++k) prev[k] = cur[k];
        }
        cache_len = need;
    }

    /* Compute a(n) by binomial convolution */
    seq_t result = 0;
    seq_t bin   = 1;              /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t prod = bin * t_cache[k + 1];   /* t_cache[1] corresponds to |s(1,4)| */
        result += prod;
        if (k < n) {
            bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return result;
}
