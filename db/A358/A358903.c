/*
 * A358903  Number of integer partitions of n whose parts have all different
 *          numbers of distinct prime factors (A001221).
 * https://oeis.org/A358903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n whose parts have all
 *            different numbers of distinct prime factors
 * Range:     u8 n <= 115, u16 n <= 836, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a group–knapsack dynamic programming approach. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_omega.h"

/* compute smallest prime factors up to n */
static void build_spf(size_t n, int *spf)
{
    size_t i, j;
    for (i = 0; i <= n; ++i) spf[i] = (int)i;
    for (i = 2; i * i <= n; ++i)
        if (spf[i] == (int)i)
            for (j = i * i; j <= n; j += i)
                if (spf[j] == (int)j)
                    spf[j] = (int)i;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;            /* use size_t for array indices */
    if (n == (idx_t)0) return (seq_t)1;  /* a(0) = 1 */

    /* 1) precompute ω(m) for 1 ≤ m ≤ n */
    int *spf   = (int*)malloc((N + 1) * sizeof(int));
    int *omega = (int*)malloc((N + 1) * sizeof(int));
    if (!spf || !omega) return (seq_t)0;  /* allocation failure */

    build_spf(N, spf);
    int max_omega;
    compute_omega(N, spf, omega, &max_omega);

    /* 2) group numbers by ω(m) */
    size_t *cnt = (size_t*)calloc(max_omega + 1, sizeof(size_t));
    int k, i;
    for (i = 1; i <= (int)N; ++i)
        cnt[omega[i]]++;

    int **grp  = (int**)malloc((max_omega + 1) * sizeof(int*));
    size_t *len = (size_t*)malloc((max_omega + 1) * sizeof(size_t));
    size_t *pos = (size_t*)calloc(max_omega + 1, sizeof(size_t));
    for (k = 0; k <= max_omega; ++k) {
        len[k] = cnt[k];
        grp[k] = (int*)calloc(len[k], sizeof(int));
    }

    for (i = 1; i <= (int)N; ++i) {
        k = omega[i];
        grp[k][pos[k]++] = i;
    }

    /* 3) group knapsack dynamic programming */
    seq_t *dp_old = (seq_t*)calloc((N + 1), sizeof(seq_t));
    seq_t *dp_new = (seq_t*)calloc((N + 1), sizeof(seq_t));
    if (!dp_old || !dp_new) return (seq_t)0;  /* allocation failure */
    dp_old[0] = (seq_t)1;

    for (k = 0; k <= max_omega; ++k) {
        /* start with “skip this group” case */
        for (size_t s = 0; s <= N; ++s)
            dp_new[s] = dp_old[s];

        /* for each candidate m in this ω‑group */
        for (size_t idx = 0; idx < len[k]; ++idx) {
            int m = grp[k][idx];
            for (int s = (int)N; s >= m; --s)
                dp_new[s] += dp_old[s - m];
        }
        /* swap dp_old and dp_new for next group */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }

    seq_t result = dp_old[N];

    /* free allocated memory */
    free(spf);
    free(omega);
    free(cnt);
    for (k = 0; k <= max_omega; ++k) free(grp[k]);
    free(grp);
    free(len);
    free(pos);
    free(dp_old);
    free(dp_new);

    return result;
}
