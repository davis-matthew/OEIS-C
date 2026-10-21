/*
 * A258396  Number of 2n-length strings of balanced parentheses of exactly 8
 *          different types that are introduced in ascending order.
 * https://oeis.org/A258396
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = catalan(n) * S(n,8) * where catalan(n) = binom(2n,
 *            n)/(n+1) * S(n,k) = k*S(n-1,k)+S(n-1,k-1) with S(0,0)=1
 * Range:     u8 none, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A258396 sequence values. */

#include <stdlib.h>    /* malloc, realloc, abort */

static seq_t *cat_tbl = NULL;              /* Catalan numbers: cat_tbl[i] = C_i */
static idx_t  cat_max = 0;                 /* largest n with C_n computed */

static seq_t *st_tbl = NULL;               /* Stirling numbers table, row major */
static idx_t  st_max = 0;                 /* largest n with S(n,*) computed */

static void ensure_catalan(idx_t n)
{
    if (n <= cat_max) return;

    size_t new_len = (size_t)(n + 1);
    size_t old_len = (size_t)(cat_max + 1);
    seq_t *ptr = cat_tbl;

    if (ptr == NULL) {                     /* initial allocation */
        ptr = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!ptr) abort();
        seq_t zero = 0;
        for (size_t i = 0; i < new_len; ++i) ptr[i] = zero;
        ptr[0] = 1;                        /* C_0 = 1 */
    } else if (new_len > old_len) {        /* grow the array */
        ptr = (seq_t *)realloc(ptr,
               new_len * sizeof(seq_t));
        if (!ptr) abort();
        seq_t zero = 0;
        for (size_t i = old_len; i < new_len; ++i) ptr[i] = zero;
    }
    cat_tbl = ptr;

    for (idx_t i = cat_max + 1; i <= n; ++i) {
        seq_t num = (seq_t)2 * ((seq_t)2 * (seq_t)i - (seq_t)1);
        seq_t den = (seq_t)(i + 1);
        cat_tbl[i] = cat_tbl[i - 1] * num / den;
    }
    cat_max = n;
}

static void ensure_stirling(idx_t n)
{
    if (n <= st_max) return;

    size_t new_rows = (size_t)(n + 1);
    size_t old_rows = (size_t)(st_max + 1);
    size_t new_len = new_rows * 9;
    size_t old_len = old_rows * 9;
    seq_t *ptr = st_tbl;

    if (ptr == NULL) {                     /* initial allocation */
        ptr = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!ptr) abort();
        seq_t zero = 0;
        for (size_t i = 0; i < new_len; ++i) ptr[i] = zero;
        ptr[0] = 1;                        /* S(0,0) = 1 */
    } else if (new_len > old_len) {         /* grow the table */
        ptr = (seq_t *)realloc(ptr,
               new_len * sizeof(seq_t));
        if (!ptr) abort();
        seq_t zero = 0;
        for (size_t i = old_len; i < new_len; ++i) ptr[i] = zero;
    }
    st_tbl = ptr;

    for (idx_t i = st_max + 1; i <= n; ++i) {
        for (idx_t k = 0; k <= 8; ++k) {
            seq_t val;
            if (k == 0) {
                val = 0; /* S(i,0) = 0 for i>0 */
            } else {
                seq_t prev_k  = st_tbl[(size_t)(i - 1) * 9 + (size_t)k];
                seq_t prev_km1 = st_tbl[(size_t)(i - 1) * 9 + (size_t)(k - 1)];
                val = (seq_t)k * prev_k + prev_km1;
            }
            st_tbl[(size_t)i * 9 + (size_t)k] = val;
        }
    }
    st_max = n;
}

seq_t a(idx_t n)
{
    if (n < 8)                 /* behaviour below offset is irrelevant */
        return 0;

    if (cat_tbl == NULL || n > cat_max)
        ensure_catalan(n);
    if (st_tbl == NULL || n > st_max)
        ensure_stirling(n);

    seq_t cat   = cat_tbl[n];
    seq_t stir  = st_tbl[(size_t)n * 9 + 8];
    return cat * stir;
}
