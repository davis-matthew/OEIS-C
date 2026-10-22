/*
 * A350893  Number of partitions of n such that (smallest part) = 2*(number of
 *          parts).
 * https://oeis.org/A350893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{m≥1, 2m²≤n} p(n‑2m², m‑1) where p(x,k) is the number
 *            of partitions of x into at most k parts (equivalently,
 *            partitions of x with largest part ≤ k)
 * Range:     u8 n <= 69, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If this file is compiled as C++, seq_t may be an arbitrary‑precision
   integer class; otherwise it is an unsigned integral type.  */

#include <stdlib.h>

static seq_t *dp_cache = NULL;          /* 1‑D table, row 0..N, column 0..K */
static idx_t dp_Nmax = 0;               /* maximum N stored in dp_cache */
static idx_t dp_Kmax = 0;               /* maximum K stored in dp_cache */
static idx_t dp_width = 0;              /* dp_Kmax + 1 */

/* Re‑allocate and recompute the partition table for all i≤N and k≤K. */
static void recompute_dp(idx_t N, idx_t K)
{
    if (N <= dp_Nmax && K <= dp_Kmax)
        return;                      /* already big enough */

    /* Free previous storage */
#ifdef __cplusplus
    delete[] dp_cache;
#else
    free(dp_cache);
#endif

    dp_Nmax = N;
    dp_Kmax = K;
    dp_width = K + 1;

    size_t total = (size_t)(N + 1) * (size_t)(dp_width);
#ifdef __cplusplus
    dp_cache = new seq_t[total];
#else
    dp_cache = malloc(total * sizeof(seq_t));
#endif
    if (!dp_cache)
        exit(1);                     /* allocation failed */

    /* Initialise */
    for (idx_t k = 0; k <= K; ++k)
        dp_cache[k] = 1;              /* p(0,k) = 1 */

    for (idx_t i = 1; i <= N; ++i)
        dp_cache[i * dp_width] = 0;   /* p(i,0) = 0 for i>0 */

    /* Dynamic programming: p(i,k) = p(i,k-1) + p(i-k,k)  (if i≥k) */
    for (idx_t k = 1; k <= K; ++k) {
        for (idx_t i = 1; i <= N; ++i) {
            seq_t val = dp_cache[i * dp_width + (k - 1)];
            if (i >= k)
                val += dp_cache[(i - k) * dp_width + k];
            dp_cache[i * dp_width + k] = val;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find maximum m such that 2*m*m ≤ n */
    idx_t mmax = 0;
    for (idx_t m = 1;; ++m) {
        if (2 * m * m > n)
            break;
        mmax = m;
    }

    /* Build partition table up to n with at most mmax parts */
    recompute_dp(n, mmax);

    seq_t ans = 0;
    for (idx_t m = 1; m <= mmax; ++m) {
        idx_t S = n - 2 * m * m;          /* remaining sum */
        idx_t k = m - 1;                  /* number of parts allowed */
        ans += dp_cache[S * dp_width + k];
    }
    return ans;
}
