/*
 * A375434  Expansion of g.f. A(x) satisfying A(x) = (1 + x*A(x)) * (1 +
 *          3*x*A(x)^2).
 * https://oeis.org/A375434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by recurrence * a(0) = 1 * a(n) = a(n-1) + 3 *
 *            Σ_{i+j=n-1} a(i)·a(j) * + 3 * Σ_{i+j+k=n-2} a(i)·a(j)·a(k) (n ≥
 *            1) * This implements the functional equation * A(x) = (1 +
 *            xA(x))·(1 + 3xA(x)²)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for memoisation */
static seq_t *a_vals      = NULL;   /* a_vals[n] = a(n)          */
static seq_t *pair_sums   = NULL;   /* pair_sums[s] = Σ_{i+j=s} a(i)·a(j) */
static idx_t  a_max_index = (idx_t)-1;   /* last computed index; -1 if none */
static size_t a_cap  = 0;   /* allocated size of a_vals      */
static size_t ps_cap = 0;   /* allocated size of pair_sums   */

/* Ensure the array at *ptr has capacity at least 'need' (zero‑filled). */
static void ensure_cap(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? need * 2 : *cap * 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    /* zero the newly allocated tail */
    for (size_t i = *cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    *ptr   = tmp;
    *cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Grow storage if needed */
    size_t need_a  = (size_t)n + 1;
    size_t need_ps = (size_t)(2 * n) + 1;
    ensure_cap(need_a,  &a_cap,  &a_vals);
    ensure_cap(need_ps, &ps_cap, &pair_sums);

    /* Initialise from scratch if not yet done */
    if (a_max_index == (idx_t)-1)
    {
        a_vals[0]     = (seq_t)1;
        pair_sums[0]  = (seq_t)1;    /* 1·1 */
        a_max_index   = 0;
    }

    /* Compute values from the next missing index up to n */
    idx_t start = a_max_index + 1;
    for (idx_t m = start; m <= n; ++m)
    {
        /* double_sum = Σ_{i+j=m-1} a(i)·a(j) = pair_sums[m-1] */
        seq_t double_sum = pair_sums[m-1];

        /* triple_sum = Σ_{i+j+k=m-2} a(i)·a(j)·a(k)
           = Σ_{k=0}^{m-2} a(k) · pair_sums[(m-2)-k] */
        seq_t triple_sum = (seq_t)0;
        if (m >= 2)
        {
            idx_t bound = m - 2;
            for (idx_t k = 0; k <= bound; ++k)
                triple_sum += a_vals[k] * pair_sums[bound - k];
        }

        seq_t a_m = a_vals[m-1] + (seq_t)3 * double_sum + (seq_t)3 * triple_sum;
        a_vals[m] = a_m;

        /* Update pair_sums with new contributions from a_m */
        for (idx_t i = 0; i < m; ++i)
        {
            seq_t prod = a_vals[i] * a_m;
            pair_sums[i + m] += prod + prod;   /* (i,m) and (m,i) */
        }
        /* contribution from (m,m) */
        seq_t prod_mm = a_m * a_m;
        pair_sums[2 * m] += prod_mm;
    }

    a_max_index = n;
    return a_vals[n];
}
