/*
 * A368025  Array read by ascending antidiagonals: A(n,k) is the determinant
 *          of the n-th order Hankel matrix of Catalan numbers M(n) whose
 *          generic element is given by M(i,j) = A000108(i+j+k) with i,j = 0,
 *          ..., n-1.
 * https://oeis.org/A368025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A368025 – Array read by ascending antidiagonals
 *  A(n,k) = determinant of the n‑th order Hankel matrix of Catalan
 *           numbers M(i,j) = Catalan(i+j+k)  (i,j = 0..n-1).
 *  The sequence is the flattening of the array A(n,k) by
 *  ascending antidiagonals (constant n+k).  Sequence index n is
 *  interpreted as the pair (i,j) where i+j = s and j = offset,
 *  following the pattern:
 *          a(0) = A(0,0) = 1
 *          a(1) = A(1,0)
 *          a(2) = A(0,1)
 *          ...
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/catalan.h"

/* Helper: allocate an array of seq_t that works in C and C++ */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(sizeof(seq_t) * n);
#endif
}

/* Helper: free an array of seq_t allocated above */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Cache for already computed terms */
static seq_t *a_cache = NULL;
static idx_t cache_cap = 0;

/* Ensure cache length >= new_cap */
static void ensure_cache_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    seq_t *new_cache = alloc_seq_array((size_t)new_cap);
    if (!new_cache) {
        /* Memory allocation failure – abort – safe for Build testing */
        exit(EXIT_FAILURE);
    }

    /* Copy old values */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = a_cache[i];

    /* Initialize new entries to a unique sentinel value (0 is fine) */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    free_seq_array(a_cache);
    a_cache = new_cache;
    cache_cap = new_cap;
}

/* Compute determinant of i x i Hankel matrix with shift j */
static seq_t hankel_det(idx_t i, idx_t j)
{
    if (i == 0)
        return (seq_t)1;          /* determinant of the empty matrix */
    if (i == 1)
        return catalan(j);        /* single element */

    /* Allocate working matrix [size][size] */
    size_t size = (size_t)i;
    seq_t *m = alloc_seq_array(size * size);
    if (!m)
        exit(EXIT_FAILURE);       /* out of memory */

    /* Fill matrix: m[r][c] = Catalan(j + r + c)  */
    for (size_t r = 0; r < size; ++r) {
        for (size_t c = 0; c < size; ++c) {
            m[r * size + c] = catalan(j + r + c);
        }
    }

    /* Bareiss algorithm – integer-preserving Gaussian elimination */
    for (size_t k = 0; k + 1 < size; ++k) {
        seq_t pivot   = m[k * size + k];
        seq_t prev_div = (k == 0) ? (seq_t)1 : m[(k - 1) * size + (k - 1)];
        for (size_t i = k + 1; i < size; ++i) {
            for (size_t j = k + 1; j < size; ++j) {
                /* (m[i][j]*pivot - m[i][k]*m[k][j]) / prev_div */
                seq_t prod1 = m[i * size + j] * pivot;
                seq_t prod2 = m[i * size + k] * m[k * size + j];
                seq_t num   = prod1 - prod2;
                m[i * size + j] = num / prev_div; /* division exact */
            }
            m[i * size + k] = (seq_t)0; /* not needed */
        }
    }

    seq_t det = m[(size - 1) * size + (size - 1)];
    free_seq_array(m);
    return det;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Return cached value if present */
    if (a_cache && n < cache_cap && a_cache[n] != (seq_t)0)
        return a_cache[n];

    /* Map linear index n to pair (i,j) via antidiagonals */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    if (s * (s + 1) / 2 > n) {
        while (s * (s + 1) / 2 > n)
            --s;
    } else {
        while ((s + 1) * (s + 2) / 2 <= n)
            ++s;
    }

    idx_t offset = n - s * (s + 1) / 2;
    idx_t i = s - offset;  /* row index (order of Hankel matrix) */
    idx_t j = offset;       /* column index (shift) */

    seq_t result;
    if (i == 0)
        result = (seq_t)1;
    else
        result = hankel_det(i, j);

    /* Cache the result */
    ensure_cache_capacity(n + 1);
    a_cache[n] = result;
    return result;
}
