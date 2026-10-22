/*
 * A352367  Row sums of A352366.
 * https://oeis.org/A352367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n≥1, a(n)=Σ_{k=1..n}
 *            binom(n‑1,k‑1)·Catalan(k‑1)·a(n‑k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Catalan(0)=1 and Catalan(m)=binom(2m,m)/(m+1)   */

#include <stdlib.h>

/* ---------- Catalan cache ---------- */
static seq_t *cat_cache = NULL;   /* cat_cache[i] = Catalan(i)   */
static size_t cat_cap  = 0;       /* allocated capacity                */
static size_t cat_size = 0;       /* number of stored Catalan numbers   */

/* Make sure cat_cache can hold Catalan numbers up to index n (inclusive). */
static void ensure_cat(idx_t n)
{
    /* Allocate if needed. */
    if (cat_cap == 0) {
        cat_cache = (seq_t *)malloc(sizeof(seq_t));
        cat_cache[0] = (seq_t)1;             /* Catalan(0) = 1 */
        cat_cap   = 1;
        cat_size  = 1;
    }

    /* Grow the allocation if required. */
    if (cat_cap <= (size_t)n) {
        size_t new_cap = cat_cap > 0 ? cat_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cat_cache, new_cap * sizeof(seq_t));
        cat_cache = tmp;
        cat_cap   = new_cap;
    }

    /* Compute missing Catalan numbers.  Recurrence:
       Catalan(i+1) = Catalan(i) * (4*i+2) / (i+2)   (integer division). */
    for (idx_t i = (idx_t)(cat_size - 1); i < n; ++i) {
        seq_t numerator   = cat_cache[i] * (seq_t)(4 * i + 2);
        seq_t denominator = (seq_t)(i + 2);
        cat_cache[i + 1] = numerator / denominator;
    }
    cat_size = (size_t)n + 1;
}

/* ---------- a(n) cache ---------- */
static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)    */
static size_t a_cap   = 0;      /* allocated capacity   */
static size_t a_size  = 0;      /* number of stored a's */

/* Make sure a_cache can hold a values up to index n (inclusive). */
static void ensure_a(idx_t n)
{
    if (a_cap == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;   /* a(0)=1 */
        a_cap   = 1;
        a_size  = 1;
    }
    if (a_cap <= (size_t)n) {
        size_t new_cap = a_cap > 0 ? a_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    ensure_a(n);

    /* We need Catalan numbers up to index n-1.  For n==0 we skip. */
    if (n > 0) ensure_cat(n - 1);

    /* Compute all a(m) we haven't yet computed, up to n. */
    for (idx_t m = a_size; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;   /* binom(m-1,0) = 1 */

        for (idx_t k = 1; k <= m; ++k) {
            /* term = binom(m-1,k-1) * Catalan(k-1) * a(m-k) */
            seq_t prod = bin * cat_cache[k - 1];
            prod *= a_cache[m - k];
            sum += prod;

            /* Prepare binom(m-1,k) for next iteration. */
            if (k < m) {
                bin = bin * (seq_t)(m - k) / (seq_t)k;
            }
        }
        a_cache[m] = sum;
    }

    a_size = (size_t)n + 1;
    return a_cache[n];
}
