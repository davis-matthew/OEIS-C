/*
 * A383477  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) is the number of lattice paths from (0,0)
 *          to (n,k) using steps (1,0),(2,0),(3,0),(0,1).
 * https://oeis.org/A383477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where A(i,j)=A(i-1,j)+A(i-2,j)+A(i-3,j)+A(i,j-1),
 *            A(0,0)=1. a(n) is read along anti‑diagonals (i+j=d) in the
 *            square array
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d such that
       d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    /* use a loop because the diagonal length is only about sqrt(2*n)
       even for n=10000 (d≈140), so the cost is negligible. */
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }

    /* The array dimension is (d+1) because indices 0…d are used. */
    idx_t dim = d + 1;
    size_t cap = (size_t)dim * (size_t)dim;

    seq_t *A;
#ifdef __cplusplus
    std::vector<seq_t> vec(cap);
    A = vec.data();        /* pointer to the underlying buffer */
#else
    A = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!A) return (seq_t)0;   /* allocation failure – return 0 as safe fallback */
#endif

    /* Fill the square array row by row. */
    for (idx_t i = 0; i <= d; ++i) {
        for (idx_t j = 0; j <= d; ++j) {
            size_t idx = (size_t)i * (size_t)dim + (size_t)j;
            if (i == 0 && j == 0) {
                A[idx] = (seq_t)1;
            } else {
                seq_t val = (seq_t)0;
                if (i >= 1) val += A[idx - (size_t)dim];
                if (i >= 2) val += A[idx - 2 * (size_t)dim];
                if (i >= 3) val += A[idx - 3 * (size_t)dim];
                if (j >= 1) val += A[idx - 1];
                A[idx] = val;
            }
        }
    }

    /* Convert index n to coordinates (i,j) on that diagonal. */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;
    idx_t j = d - offset;
    seq_t result = A[(size_t)i * (size_t)dim + (size_t)j];

#ifndef __cplusplus
    free(A);
#endif
    return result;
}
