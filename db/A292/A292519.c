/*
 * A292519  Expansion of Product_{k>=1} 1/(1 + x^(k*(k+1)/2)).
 * https://oeis.org/A292519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Product_{k>=1} 1/(1 + x^(k(k+1)/2))
 * Range:     i8 n <= 131, i16 n <= 581, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    /* Return 1 for n = 0 */
    if (n == 0) return (seq_t)1;

    /* Allocate an array a[0..n] */
    idx_t sz = n + 1;
    seq_t *arr;
#ifdef __cplusplus
    /* C++: use std::vector for proper construction/destruction */
    std::vector<seq_t> vec(sz, 0);
    arr = vec.data();
#else
    /* C: use calloc (pseudo–trivial types only) */
    arr = calloc((size_t)sz, sizeof(seq_t));
    if (!arr) return 0;          /* fallback: out‑of‑memory */
#endif

    arr[0] = (seq_t)1;

    /* Iterate over triangular numbers t = k(k+1)/2 */
    for (idx_t k = 1;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;
        /* In‑place update: a[i] = a[i] - a[i - t] for i >= t */
        for (idx_t i = t; i <= n; ++i) {
            arr[i] = arr[i] - arr[i - t];
        }
    }

#ifdef __cplusplus
    /* For C++ the vector will be destroyed when leaving the function */
    return arr[n];
#else
    seq_t result = arr[n];
    free(arr);
    return result;
#endif
}
