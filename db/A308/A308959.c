/*
 * A308959  Sum of the second largest parts in the partitions of n into 7
 *          squarefree parts.
 * https://oeis.org/A308959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Sum of the second largest parts in the partitions of n into 7 squarefree parts.
 * a(n) = Sum_{L squarefree, 1 <= L <= n} 
 *          Sum_{partitions of n-L into 6 nondecreasing squarefree parts 
 *          with each part <= L} (maximum of the 6 parts)
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: compute least prime factor for each number up to n */
static void compute_spf(idx_t n, idx_t *spf) {
    idx_t i, j;
    for (i = 2; i <= n; ++i) spf[i] = i;
    for (i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

/* Helper: compute Möbius function values up to n (stored in int array) */
static void compute_mobius(idx_t n, int *mu, const idx_t *spf) {
    mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;
        int mu_val = 1;
        int prev_p = 0;
        int multiple = 0;
        while (x > 1) {
            idx_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
                if (cnt > 1) break;
            }
            if (cnt > 1) {          /* square factor found */
                mu_val = 0;
                break;
            }
            if (p == prev_p) {       /* repeated prime factor (shouldn't happen because cnt==1) */
                mu_val = 0;
                break;
            }
            mu_val = -mu_val;
            prev_p = p;
        }
        mu[i] = mu_val;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 7) return 0;

    size_t N = (size_t)n;
    /* Allocate temporary arrays for μ and spf */
    int   *mu  = (int*)   calloc(N + 1, sizeof(int));
    idx_t *spf = (idx_t*) calloc(N + 1, sizeof(idx_t));
    if (!mu || !spf) return 0;   /* allocation failure guard */

    compute_spf(n, spf);
    compute_mobius(n, mu, spf);

    /* Build list of squarefree numbers up to n */
    int *sf = (int*)calloc(N + 1, sizeof(int));
    size_t sf_count = 0;
    for (size_t i = 1; i <= N; ++i) {
        if (mu[i] != 0) sf[sf_count++] = (int)i;
    }

    /* DP arrays: count and sum of maximums for m parts */
    size_t rows = 7;               /* we need m = 0..6 */
    size_t cols = N + 1;
    seq_t *cnt   = (seq_t*)calloc(rows * cols, sizeof(seq_t));
    seq_t *sumdp = (seq_t*)calloc(rows * cols, sizeof(seq_t));
    if (!cnt || !sumdp) return 0;   /* allocation failure guard */

    cnt[0] = 1;   /* one way to sum to 0 with 0 parts */

    seq_t result = 0;

    for (size_t idx = 0; idx < sf_count; ++idx) {
        int v = sf[idx];             /* current largest part candidate */
        size_t v_sz = (size_t)v;     /* cast for indexing */

        for (int m = 1; m <= 6; ++m) {
            size_t base_prev = (m - 1) * cols;
            size_t base_cur  = m * cols;
            for (size_t s = v_sz; s <= N; ++s) {
                seq_t addCnt = cnt[base_prev + (s - v_sz)];
                if (addCnt) {
                    cnt[base_cur + s]   += addCnt;
                    sumdp[base_cur + s] += addCnt * (seq_t)v;
                }
            }
        }

        /* After including all parts up to v, contribute to the result */
        size_t remain = N - v_sz;          /* sum remaining after choosing largest part v */
        seq_t contrib = sumdp[6 * cols + remain];
        result += contrib;
    }

    /* Free allocated memory */
    free(mu);
    free(spf);
    free(sf);
    free(cnt);
    free(sumdp);

    return result;
}
