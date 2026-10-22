/*
 * A346818  Number of partitions of the (n+8)-multiset {0,...,0,1,2,...,n}
 *          with eight 0's into distinct multisets.
 * https://oeis.org/A346818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n+1} S(n+1,k) * Σ_{s=0}^8 C(k-1+s,s) * p_d[8-s];
 *            where S are Stirling numbers of the second kind, p_d[s] = number
 *            of partitions of s into distinct parts (p_d[0]=1). a(0)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static const unsigned char p_distinct[9] = {1,1,1,2,2,3,4,5,6};

static seq_t *a_cache   = NULL;    /* a[0..max_n]          */
static size_t a_cap     = 0;
static seq_t *B_cache   = NULL;    /* B[0..max_n]          */
static size_t B_cap     = 0;
static seq_t *row_prev  = NULL;    /* S(prev_n, k)          */
static seq_t *row_curr  = NULL;    /* temporary row         */
static size_t row_cap   = 0;
static idx_t  max_n     = 0;       /* largest n computed so far */

/* Ensure arrays are large enough */
static void ensure_a(size_t n)
{
    if (n < a_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    /* zero new entries to be safe */
    for (size_t i = a_cap; i < new_cap; ++i) new_arr[i] = 0;
    a_cache = new_arr;
    a_cap   = new_cap;
}

static void ensure_B(size_t n)
{
    if (n < B_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_arr = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    for (size_t i = B_cap; i < new_cap; ++i) new_arr[i] = 0;
    B_cache = new_arr;
    /* compute new B's */
    for (size_t k = B_cap; k <= n; ++k) {
        if (k == 0) { B_cache[0] = 0; continue; }
        seq_t binom = 1;          /* C(k-1+0,0) */
        seq_t sum   = 0;
        for (int s = 0; s <= 8; ++s) {
            if (s > 0) {
                binom = binom * ((seq_t)k - 1 + s) / (seq_t)s; /* C(k-1+s,s) */
            }
            sum += binom * (seq_t)p_distinct[8 - s];
        }
        B_cache[k] = sum;
    }
    B_cap = new_cap;
}

static void ensure_rows(size_t n)
{
    if (n < row_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_prev, *new_curr;
    if (row_prev == NULL) {
        new_prev = (seq_t *)malloc(new_cap * sizeof(seq_t));
        new_curr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_prev || !new_curr) abort();
    } else {
        new_prev = (seq_t *)realloc(row_prev, new_cap * sizeof(seq_t));
        new_curr = (seq_t *)realloc(row_curr, new_cap * sizeof(seq_t));
        if (!new_prev || !new_curr) abort();
    }
    /* zero new elements beyond old capacity */
    if (row_prev) memset(new_prev + row_cap, 0, (new_cap - row_cap) * sizeof(seq_t));
    if (row_curr) memset(new_curr + row_cap, 0, (new_cap - row_cap) * sizeof(seq_t));
    row_prev = new_prev;
    row_curr = new_curr;
    row_cap  = new_cap;
    /* for the very first time initialise the base Stirling row */
    if (max_n == 0) {
        row_prev[0] = (seq_t)1;
        if (row_cap > 1) row_prev[1] = (seq_t)0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;  /* a(0) = 6 */

    /* Expand cached data if needed */
    if ((idx_t)n > max_n) {
        ensure_a((size_t)n);
        ensure_B((size_t)n);
        ensure_rows((size_t)n);

        /* Initialise a_cache[0] if not already done */
        if (max_n == 0) a_cache[0] = (seq_t)6;

        /* Compute rows and a values from max_n+1 up to n */
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            /* Compute Stirling row S(cur,k) */
            row_curr[0] = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t term1 = (seq_t)k * row_prev[k];
                seq_t term2 = row_prev[k - 1];
                row_curr[k] = term1 + term2;
            }

            /* Compute a(cur) = Σ_{k=1..cur} S(cur,k) * B(k) */
            seq_t acc = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                acc += row_curr[k] * B_cache[k];
            }
            a_cache[cur] = acc;

            /* Prepare for next iteration */
            seq_t *tmp = row_prev;
            row_prev  = row_curr;
            row_curr  = tmp;
        }

        max_n = n;
    }

    return a_cache[(size_t)n];
}
