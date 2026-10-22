/*
 * A326445  Sum of the smallest parts of the partitions of n into 8 squarefree
 *          parts.
 * https://oeis.org/A326445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the smallest parts of the partitions of n * into 8
 *            squarefree parts. The implementation uses a dynamic *
 *            programming approach that keeps, for every sum t and number of
 *            parts j, * the number of partitions and the total sum of the
 *            smallest parts. * The partitions are counted in non‑de...
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External types provided by the build system. */
seq_t a(idx_t n);

typedef struct {
    idx_t max_n;   /* largest n for which a[] is valid */
    seq_t *a;       /* array of a(n) for 0 .. max_n */
} cache_t;

static cache_t cache = {0, NULL};

/* ------------------------------------------------------------------ */
/* Helper: free previously allocated cache.                            */
static void free_cache(void) {
    if (cache.a) {
        free(cache.a);
        cache.a = NULL;
    }
    cache.max_n = 0;
}

/* ------------------------------------------------------------------ */
/* Recompute a[0..N] together with the auxiliary tables.               */
static void recompute(idx_t N)
{
    /* --- 1.  Prepare squarefree flag array --------------------------------- */
    bool *is_sf = (bool *)calloc((size_t)(N + 1), sizeof(bool));
    if (!is_sf) return;      /* allocation failure – caller will handle */
    for (idx_t i = 1; i <= N; ++i) is_sf[i] = true;
    for (idx_t i = 2, sq = i * i; sq <= N; ++i, sq = i * i)
        if (is_sf[i])
            for (idx_t j = sq; j <= N; j += sq)
                is_sf[j] = false;

    /* --- 2.  Allocate DP tables ------------------------------------------ */
    const size_t stride = (size_t)(N + 1);     /* columns for all sums 0..N */
    const size_t rows   = 9;                  /* j = 0 .. 8 */
    seq_t *dp = (seq_t *)calloc(rows * stride, sizeof(seq_t));
    seq_t *S  = (seq_t *)calloc(rows * stride, sizeof(seq_t));
    if (!dp || !S) {
        free(is_sf); free(dp); free(S);
        return;
    }

    /* Base case: empty partition, 0 parts, sum 0, count = 1, sum of smallest = 0 */
    dp[0 * stride + 0] = (seq_t)1;   /* dp[0][0] = 1 */

    /* --- 3.  Iterate over admissible part sizes (squarefree) ----------------- */
    for (idx_t v = 1; v <= N; ++v) {
        if (!is_sf[v]) continue;   /* skip non‑squarefree numbers */

        for (int j = 1; j <= 8; ++j) {          /* number of parts built so far */
            size_t base_j   = (size_t)j * stride;
            size_t base_j1  = (size_t)(j - 1) * stride;
            for (idx_t t = v; t <= N; ++t) {   /* total sum of the new partition */
                size_t idx      = base_j + t;
                size_t idx_prev = base_j1 + (t - v);

                /* Update counts */
                dp[idx] += dp[idx_prev];

                /* Update sum of smallest parts */
                if (j == 1)
                    /* The partition consists solely of v -> smallest is v */
                    S[idx] += (seq_t)v * dp[idx_prev];
                else
                    S[idx] += S[idx_prev];
            }
        }
    }

    /* --- 4.  Extract a(n) from the table for exactly 8 parts ---------------- */
    seq_t *a_arr = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (a_arr) {
        size_t base8 = 8 * stride;
        for (idx_t i = 0; i <= N; ++i)
            a_arr[i] = S[base8 + i];
    }

    /* --- 5.  Clean up temporary arrays -------------------------------------- */
    free(is_sf); free(dp); free(S);

    /* --- 6.  Update cache (free old data if present) ------------------------ */
    free_cache();
    if (a_arr) {
        cache.a    = a_arr;
        cache.max_n = N;
    }
}

/* ------------------------------------------------------------------ */
/* Public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n <= cache.max_n && cache.a)
        return cache.a[n];

    /* Compute up to n (or higher if needed) */
    recompute(n);

    /* After recompute, cache.a must be valid */
    if (cache.a && n <= cache.max_n)
        return cache.a[n];

    /* In the unlikely event of a failure, return 0 */ 
    return (seq_t)0;
}
