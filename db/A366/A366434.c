/*
 * A366434  G.f. A(x) satisfies A(x) = 1 + x * ((1 - x) / A(x))^2.
 * https://oeis.org/A366434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A366434: a(n) = (-1)^(n-1) * Sum_{k=0..n} binomial(3*k-1,k) * binomial(2*k,n-k) / (3*k-1)
 *
 * The sequence satisfies the functional equation
 *     A(x) = 1 + x * ((1 - x) / A(x))^2
 *
 * The following implementation uses the equivalent recurrence derived from
 * the equation:
 *     a(n) = rhs(n) - sum_{i=1}^{n-1} a(i) * c(n-i)
 * where
 *     rhs(1) =  1,  rhs(2) = -2,  rhs(3) =  1,  rhs(n)=0 for n>3
 *     c(m) = sum_{j=0}^m a(j) * a(m-j)   (convolution of the sequence with itself)
 *
 * This recurrence only requires values a(i) and c(i) for i < n and
 * runs in O(n^2) time, which is fast enough for n up to about 10 000.
 *
 * The function keeps a memoised cache so that calls can be made in any
 * order without recomputing already known terms.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *a_cache = NULL; /* a values */
    static seq_t *c_cache = NULL; /* convolution values: c[m] = sum_{j=0}^m a[j]*a[m-j] */
    static idx_t cache_size = 0;  /* allocated size of the arrays          */
    static idx_t max_computed = 0;/* number of terms already computed      */

    /* initialise the cache on first call */
    if (max_computed == 0) {
        cache_size = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        c_cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        /* a(0) = 1 */
        a_cache[0] = 1;
        c_cache[0] = 1;   /* a(0)*a(0) */
        max_computed = 1; /* only a(0) known */
    }

    /* enlarge cache if needed */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        a_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
        c_cache = (seq_t*)realloc(c_cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute terms up to n (if not already computed) */
    for (idx_t k = max_computed; k <= n; ++k) {
        /* RHS of the recurrence */
        seq_t rhs;
        if (k == 1)        rhs =  1;
        else if (k == 2)   rhs = -2;
        else if (k == 3)   rhs =  1;
        else               rhs =  0;

        seq_t sum_i = 0;   /* sum_{i=1}^{k-1} a(i) * c(k-i)   */
        seq_t sum_c = 0;   /* sum_{i=1}^{k-1} a(i) * a(k-i) = c'_k */

        for (idx_t i = 1; i < k; ++i) {
            sum_i += a_cache[i] * c_cache[k - i];
            sum_c += a_cache[i] * a_cache[k - i];
        }

        a_cache[k] = rhs - sum_i;
        c_cache[k] = sum_c + (a_cache[k] << 1); /* 2*a(k) */
    }

    max_computed = n + 1;
    return a_cache[n];
}
