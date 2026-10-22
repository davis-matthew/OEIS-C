/*
 * A317019  Expansion of Product_{k>=1} 1/(1 - x^k)^(k*binomial(k+2,3)).
 * https://oeis.org/A317019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1 a(n) = (1/n) * Σ_{k=1}^n ( Σ_{d|k} d *
 *            A002417(d) ) * a(n-k), where A002417(k) = k * binomial(k+2,3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */
static seq_t *A = NULL;      /* a[0..]          */
static seq_t *V = NULL;      /* V[k] = Σ_{d|k} d*A002417(d) */
static size_t cap = 0;       /* current allocation size   */
static idx_t cur_n = 0;      /* largest n for which A[n] is known */

/* Compute V[k] for k = old_cap .. n and store in V. */
static void compute_V(idx_t start, idx_t n)
{
    for (idx_t k = start; k <= n; ++k)
    {
        if (k == 0) { V[0] = (seq_t)0; continue; }
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d)
        {
            if (k % d == 0)
            {
                seq_t dd  = (seq_t)d;
                seq_t bin = ((dd + (seq_t)2) * (dd + (seq_t)1) * dd) / (seq_t)6;
                seq_t ud  = dd * bin;         /* A002417(d)      */
                sum += dd * ud;               /* d * A002417(d)  */
                idx_t d2 = k / d;
                if (d2 != d)
                {
                    seq_t dd2 = (seq_t)d2;
                    seq_t bin2 = ((dd2 + (seq_t)2) * (dd2 + (seq_t)1) * dd2) / (seq_t)6;
                    seq_t ud2  = dd2 * bin2;
                    sum += dd2 * ud2;
                }
            }
        }
        V[k] = sum;
    }
}

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (cap == 0)
    {
        cap = 1;
        A = (seq_t *)malloc(sizeof(seq_t));
        V = (seq_t *)malloc(sizeof(seq_t));
        if (!A || !V) exit(EXIT_FAILURE);
        A[0] = (seq_t)1;
        V[0] = (seq_t)0;
        cur_n = 0;
    }

    if (n <= cur_n) return A[n];

    /* Extend arrays if necessary and compute new V values */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap)
    {
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        V = (seq_t *)realloc(V, new_cap * sizeof(seq_t));
        if (!A || !V) exit(EXIT_FAILURE);
        for (size_t i = cap; i < new_cap; ++i)
            A[i] = (seq_t)0;     /* A will be computed below */
        compute_V((idx_t)cap, n);
        cap = new_cap;
    }

    /* Compute a[n] using the recurrence */
    for (idx_t nn = cur_n + 1; nn <= n; ++nn)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= nn; ++k)
            sum += V[k] * A[nn - k];
        A[nn] = sum / (seq_t)nn;   /* division is exact */
    }
    cur_n = n;
    return A[n];
}
