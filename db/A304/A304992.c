/*
 * A304992  G.f.: Sum_{k>=0} A000041(k)^3 * x^k / Sum_{k>=0} A000009(k) * x^k.
 * https://oeis.org/A304992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑ A000041(k)^3 x^k) / (∑
 *            A000009(k) x^k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304992.  The code computes the sequence by generating the two
   necessary series (partitions and distinct-partitions) and then inverting
   the denominator via the recurrence
        a[n] = A000041(n)^3 – Σ_{k=1}^{n} A000009(k)·a[n‑k].  */

/* The implementation uses dynamic allocation of arrays of type seq_t.
   It must be compiled as C11 or C++17.  The helper functions
   alloc_seq_array and free_seq_array are provided by the utilities
   library.  No other external dependencies are required.  */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate arrays for partitions, distinct partitions, and the target sequence. */
    seq_t *p = alloc_seq_array((size_t)(n + 1));
    seq_t *d = alloc_seq_array((size_t)(n + 1));
    seq_t *v = alloc_seq_array((size_t)(n + 1));
    if (!p || !d || !v)   /* allocation failed – return 0 */
        return 0;

    /* --------- 1. Two series --------------------------------------- */
    /* Initialise arrays */
    p[0] = 1;
    d[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        p[i] = 0;
        d[i] = 0;
        v[i] = 0;          /* v will hold a[] values */
    }

    /* Partition numbers A000041 via the standard additive DP. */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; ++j)
            p[j] += p[j - i];

    /* Distinct partitions A000009 via subset-sum DP (each part used at most once). */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = n; j >= i; --j)
            d[j] += d[j - i];

    /* --------- 2. Sequence a(n) via the recurrence ---------------- */
    /* a[0] = A000041(0)^3 = 1^3 = 1 */
    seq_t cube0 = p[0];
    cube0 = cube0 * cube0;
    cube0 = cube0 * p[0];
    v[0] = cube0;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += d[k] * v[m - k];
        seq_t cube = p[m];
        cube = cube * cube;
        cube = cube * p[m];
        v[m] = cube - sum;
    }

    seq_t result = v[n];

    /* Free allocated memory */
    free_seq_array(p);
    free_seq_array(d);
    free_seq_array(v);

    return result;
}
