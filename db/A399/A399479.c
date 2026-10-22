/*
 * A399479  Triangular array read by rows. T(n,k) is the number of labeled
 *          acyclic digraphs on [n] that have exactly k isolated points, n>=0,
 *          0<=k<=n.
 * https://oeis.org/A399479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is obtained by reading the triangular array T(n,k)
 *            row‑wise, where T(n,k) is the number of labeled acyclic digraphs
 *            on n vertices having exactly k isolated points (0 ≤ k ≤ n). The
 *            function below implements the following computation: a(n) =
 *            T(r,k) with n = r(r+1)/2 + k, where r is the...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 65, u128 n <= 119,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Global cache for B[n] (total number of acyclic digraphs). */
static seq_t *B_cache = NULL;
static size_t B_cap   = 0;   /* allocated size of B_cache */
static idx_t  B_len   = 0;   /* number of entries computed (max index+1) */

/* Ensure B_cache has entries 0..needed inclusive. */
static void ensure_B(idx_t needed)
{
    if (needed < B_len)
        return;

    /* enlarge allocation to at least needed+1. */
    size_t new_cap = B_cap ? B_cap : 1;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* allocation failure */
    B_cache = tmp;
    B_cap = new_cap;

    /* compute missing entries */
    for (idx_t i = B_len; i <= needed; ++i) {
        if (i == 0) {
            B_cache[0] = 1;
            continue;
        }
        seq_t pos_sum = 0, neg_sum = 0;   /* even/odd k => positive/negative terms */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial(i, k);
            term *= pow2(k * (i - k));                /* 2^{k(i-k)} */
            term *= B_cache[i - k];
            if (k & 1)
                pos_sum += term;                     /* (-1)^(k+1) : positive when k odd */
            else
                neg_sum += term;                     /* negative when k even   */
        }
        B_cache[i] = pos_sum - neg_sum;
    }
    B_len = needed + 1;   /* now indices 0..needed are valid */
}

/* Convert flattened index n to row r and position k. */
static void row_and_pos(idx_t n, idx_t *row, idx_t *pos)
{
    idx_t r = 0;
    /* Find largest r such that r(r+1)/2 <= n. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    *row = r;
    *pos = n - r * (r + 1) / 2;
}

/* Compute the value a(n). */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_and_pos(n, &r, &k);
    idx_t m = r - k;          /* number of non‑isolated vertices */

    ensure_B(r);              /* ensure total acyclic digraph counts available */

    seq_t bin_r_k = binomial(r, k);

    /* Compute D(m) via inclusion‑exclusion. */
    seq_t even_sum = 0, odd_sum = 0;
    for (idx_t j = 0; j <= m; ++j) {
        seq_t term = binomial(m, j);
        term *= B_cache[m - j];
        if (j & 1)
            odd_sum += term;
        else
            even_sum += term;
    }
    seq_t D = even_sum - odd_sum;   /* even_sum >= odd_sum for all m */

    return bin_r_k * D;
}
