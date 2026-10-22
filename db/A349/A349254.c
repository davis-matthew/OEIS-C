/*
 * A349254  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 3 * x * A(x)^2)).
 * https://oeis.org/A349254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3 * Σ_{i=0}^{n-1} Σ_{j=0}^{n-i-1}
 *            a(i)·a(j)·a(n-i-j-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Resize an array of seq_t to at least 'needed' elements.
   The new elements are zero‑initialized. */
static void ensure_capacity(seq_t **arr, idx_t *cap, idx_t needed)
{
    if (needed <= *cap) return;
    idx_t new_cap = (*cap > 0) ? *cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    if (new_cap > *cap)
        memset(tmp + *cap, 0, (new_cap - *cap) * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t A_cap = 0;
    static idx_t A_len = 0;          /* number of elements actually stored */

    static seq_t *conv2 = NULL;
    static idx_t conv_cap = 0;
    static idx_t conv_len = 0;       /* up to 2*max_index of A actually stored */

    /* Initialise arrays on first call */
    if (A_len == 0)
    {
        ensure_capacity(&A, &A_cap, 1);
        A[0] = 1;
        A_len = 1;

        ensure_capacity(&conv2, &conv_cap, 1);
        conv2[0] = 1;                /* a(0) * a(0) */
        conv_len = 1;
    }

    /* Ensure we can store A[0..n] and conv2[0..2*n] */
    ensure_capacity(&A, &A_cap, n + 1);
    ensure_capacity(&conv2, &conv_cap, 2 * n + 1);

    /* Compute new terms up to n */
    for (idx_t m = A_len; m <= n; ++m)
    {
        /* Compute the sum Σ_{i=0}^{m-1} a(i) * conv2[m-1-i] */
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i)
            sum += A[i] * conv2[m - 1 - i];

        /* New value a(m) */
        seq_t val = 1 + 3 * sum;
        A[m] = val;

        /* Update conv2 with pairs that involve the new index m.
           For each i < m: conv2[i+m] += 2 * A[i] * val
           Additionally, the pair (m,m): conv2[2*m] += val * val */
        for (idx_t i = 0; i < m; ++i)
        {
            seq_t prod = A[i] * val;
            conv2[i + m] += 2 * prod;
        }
        conv2[2 * m] += val * val;
    }
    A_len = n + 1;

    return A[n];
}
