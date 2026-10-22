/*
 * A349532  G.f. A(x) satisfies: A(x) = 1 + x * A(x)^3 / (1 - 4 * x).
 * https://oeis.org/A349532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = 4*a(n-1) + Σ_{i=0}^{n-1} Σ_{j=0}^{n-i-1}
 *            a(i)·a(j)·a(n-i-j-1) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are defined by the build environment. */

/* static cache for computed values */
static seq_t *a_cache = NULL;   /* a[0..max_n]    */
static seq_t *b_cache = NULL;   /* b[0..2*max_n] */
static size_t a_cap = 0;
static size_t b_cap = 0;
static idx_t max_computed = (idx_t)-1;   /* (idx_t)-1 indicates uninitialized */

/* ensure that an array has capacity at least 'need' elements */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    /* new slots are uninitialized; zero them */
    for (size_t i = *cap; i < new_cap; ++i)
        (*arr)[i] = 0;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Special handling for the first two terms */
    if (max_computed == (idx_t)-1) {
        /* Allocate initial capacity for the arrays */
        ensure_capacity(&a_cache, &a_cap, 3);
        ensure_capacity(&b_cache, &b_cap, 5);

        a_cache[0] = 1;
        max_computed = 0;

        /* Initialize b for a[0] */
        b_cache[0] = a_cache[0] * a_cache[0]; /* 1*1 = 1 */
    }

    if (n <= max_computed)
        return a_cache[n];

    /* Resize arrays up to needed size */
    ensure_capacity(&a_cache, &a_cap, n + 1);
    ensure_capacity(&b_cache, &b_cap, 2 * n + 1);

    /* Compute missing terms up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        if (m == 1) {
            /* a(1) = 1 by definition */
            a_cache[1] = 1;
            /* Update convolution array b for the new term */
            for (idx_t i = 0; i < m; ++i) {
                /* i runs 0 .. 0 */
                b_cache[i + m] += 2 * a_cache[i] * a_cache[m];
            }
            b_cache[2 * m] += a_cache[m] * a_cache[m];
        } else {  /* m >= 2 */
            seq_t sum = 0;
            /* sum_{k=0}^{m-1} a[k] * b[(m-1)-k] */
            for (idx_t k = 0; k < m; ++k) {
                sum += a_cache[k] * b_cache[(m - 1) - k];
            }
            a_cache[m] = 4 * a_cache[m - 1] + sum;

            /* Update convolution array b for the new term */
            for (idx_t i = 0; i < m; ++i) {
                b_cache[i + m] += 2 * a_cache[i] * a_cache[m];
            }
            b_cache[2 * m] += a_cache[m] * a_cache[m];
        }
    }

    max_computed = n;
    return a_cache[n];
}
