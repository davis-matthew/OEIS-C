/*
 * A265167  Number of n X 2 arrays containing 2 copies of 0..n-1 with no equal
 *          horizontal or vertical neighbors and new values introduced
 *          sequentially from 0.
 * https://oeis.org/A265167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (2*n-2*k-1)!! * A046741(n,k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046741(n,k) = number of matchings of size k in the ladder graph P₂×Pₙ. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Convert n to a native integer for array sizing. */
    size_t N = (size_t)n;

    /* Pre‑compute double‑factorials (2*m-1)!! for m = 0..N. */
    seq_t *df = (seq_t*)calloc(N + 1, sizeof(seq_t));
    df[0] = 1;
    for (size_t m = 1; m <= N; ++m) {
        df[m] = df[m - 1] * (seq_t)(2 * m - 1);
    }

    /* DP arrays for the four possible frontier masks. */
    seq_t *dp0 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp1 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp2 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp3 = (seq_t*)calloc(N + 1, sizeof(seq_t));

    seq_t *next0 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *next1 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *next2 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *next3 = (seq_t*)calloc(N + 1, sizeof(seq_t));

    /* Starting state: no pending edges, 0 edges used. */
    dp0[0] = 1;

    for (size_t col = 0; col < N; ++col) {
        /* Zero the next frontier. */
        for (size_t i = 0; i <= N; ++i) {
            next0[i] = next1[i] = next2[i] = next3[i] = 0;
        }

        /* Transition from mask 0 (both vertices free). */
        for (size_t k = 0; k <= N; ++k) {
            seq_t val = dp0[k];
            if (!val) continue;

            /* 0 new edges. */
            next0[k] += val;

            /* 1 edge: vertical inside the column. */
            if (k + 1 <= N) next0[k + 1] += val;

            /* 1 edge: top to the right. */
            if (k + 1 <= N) next1[k + 1] += val;

            /* 1 edge: bottom to the right. */
            if (k + 1 <= N) next2[k + 1] += val;

            /* 2 edges: top and bottom both to the right. */
            if (k + 2 <= N) next3[k + 2] += val;
        }

        /* Transition from mask 1 (top matched to the left). */
        for (size_t k = 0; k <= N; ++k) {
            seq_t val = dp1[k];
            if (!val) continue;

            /* bottom unmatched. */
            next0[k] += val;

            /* bottom matched to the right. */
            if (k + 1 <= N) next2[k + 1] += val;
        }

        /* Transition from mask 2 (bottom matched to the left). */
        for (size_t k = 0; k <= N; ++k) {
            seq_t val = dp2[k];
            if (!val) continue;

            next0[k] += val;

            if (k + 1 <= N) next1[k + 1] += val;
        }

        /* Transition from mask 3 (both matched to the left). */
        for (size_t k = 0; k <= N; ++k) {
            seq_t val = dp3[k];
            if (!val) continue;

            next0[k] += val;
        }

        /* Swap current and next DP tables. */
        seq_t *tmp;
        tmp = dp0; dp0 = next0; next0 = tmp;
        tmp = dp1; dp1 = next1; next1 = tmp;
        tmp = dp2; dp2 = next2; next2 = tmp;
        tmp = dp3; dp3 = next3; next3 = tmp;
    }

    /* At this stage, dp0 contains the numbers of matchings of every size k. */
    seq_t result = 0;
    for (size_t k = 0; k <= N; ++k) {
        seq_t term = dp0[k] * df[N - k];
        if ((k & 1ULL) == 0)
            result += term;   /* add for even k */
        else
            result -= term;   /* subtract for odd k */
    }

    /* Free all allocated memory. */
    free(df);
    free(dp0); free(dp1); free(dp2); free(dp3);
    free(next0); free(next1); free(next2); free(next3);

    return result;
}
