/*
 * A360102  a(n) = Sum_{k=0..n} binomial(n+2*k,n-k) * Catalan(k).
 * https://oeis.org/A360102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360102: a(n) = Sum_{k=0..n} binomial(n+2*k, n-k) * Catalan(k) */
/* (D-finite recurrence given in OEIS is incorrect for n≥6) */

#include <stdlib.h>
#include <string.h>

/* forward references to helper routines that are provided elsewhere */
extern seq_t catalan(idx_t k);

/* ====================================================================== */
/* 1. static cache for the sequence a(n)                                    */
static seq_t *a_cache = NULL;        /* a_cache[i] = a(i)          */
static idx_t  a_len   = 0;           /* number of stored elements   */
static idx_t  a_cap   = 0;           /* allocated capacity          */

/* 2. static cache for Catalan numbers                                     */
static seq_t *cat_cache = NULL;      /* cat_cache[i] = Catalan(i)   */
static idx_t  cat_len   = 0;          /* number of stored Catalans   */
static idx_t  cat_cap   = 0;          /* allocated capacity          */

/* ---------------------------------------------------------------------- */
/* Ensures the cache for a(n) can hold indices < need                    */
static void a_ensure_capacity(idx_t need)
{
    if (a_cap >= need) return;
    idx_t new_cap = a_cap ? a_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (a_cache) {
        memcpy(new_cache, a_cache, a_len * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_cache;
    a_cap   = new_cap;
}

/* Ensures the cache for Catalan numbers can hold indices < need          */
static void cat_ensure_capacity(idx_t need)
{
    if (cat_cap >= need) return;
    idx_t new_cap = cat_cap ? cat_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cat_cache) {
        memcpy(new_cache, cat_cache, cat_len * sizeof(seq_t));
        free(cat_cache);
    }
    cat_cache = new_cache;
    cat_cap   = new_cap;
}

/* ---------------------------------------------------------------------- */
/* Pre‑computes Catalan numbers up to 'n' (inclusive) using the standard
   recurrence:  Catalan(0)=1,  Catalan(n+1)=Σ_{i=0}^{n} Catalan(i)*Catalan(n-i). */
static void ensure_catalan(idx_t n)
{
    if (cat_len > n) return;
    if (cat_len == 0) {
        cat_ensure_capacity(1);
        cat_cache[0] = (seq_t)1;
        cat_len = 1;
    }
    for (idx_t m = cat_len; m <= n; ++m) {
        cat_ensure_capacity(m + 1);
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < m; ++i) {
            sum += cat_cache[i] * cat_cache[m - 1 - i];
        }
        cat_cache[m] = sum;
    }
    cat_len = n + 1;
}

/* ---------------------------------------------------------------------- */
/* Main function providing the sequence A360102.                          */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < a_len) return a_cache[n];

    /* Ensure all Catalan numbers up to n are available. */
    ensure_catalan(n);

    /* Grow cache for a(n). */
    a_ensure_capacity(n + 1);
    for (idx_t j = a_len; j <= n; ++j) {
        seq_t sum = (seq_t)0;
        seq_t B   = (seq_t)1;              /* B₀ = binomial(j, j) = 1 */
        /* Iterate over k = 0 .. j */
        for (idx_t k = 0; k <= j; ++k) {
            sum += B * cat_cache[k];

            if (k == j) break;            /* no further B needed */

            /* Compute B_{k+1} from B_k using:                */
            /* B_{k+1} = B_k * (j+2k+2)(j+2k+1)(j-k) /          */
            /*           (3k+3)(3k+2)(3k+1)                      */
            seq_t num = (seq_t)(j + 2 * k + 2) *
                        (seq_t)(j + 2 * k + 1) *
                        (seq_t)(j - k);
            seq_t den = (seq_t)(3 * k + 3) *
                        (seq_t)(3 * k + 2) *
                        (seq_t)(3 * k + 1);
            B = B * num / den;
        }
        a_cache[j] = sum;
    }
    a_len = n + 1;
    return a_cache[n];
}
