/*
 * A396981  Array read by ascending antidiagonals: A(n, k) = A(n,
 *          k-A002024(k)) + (n+A002024(k)-1) * A(n-1, k) with A(n, 0) = 0^n,
 *          A(0, k) = 1.
 * https://oeis.org/A396981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) read from array A(n,k) defined by * A(0,k) = 1, * A(n,0) =
 *            0^n (so A(0,0)=1, A(n>0,0)=0), * A(n,k) = A(n, k - f(k)) + (n +
 *            f(k) - 1) * A(n-1, k), * where f(k) = A002024(k) =
 *            floor((sqrt(8*k+1)-1)/2). * * The sequence starts:
 *            1,0,1,0,1,1,0,2,2,1,0,6,6,3,1,0,24,24,11,4,1,0,120,120,
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 105, u64 n <= 253,
 *            u128 n <= 666, big any n
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
    if (n == 0)
        return (seq_t)1;

    /* Find the diagonal S such that (S+1)(S+2)/2 > n. */
    idx_t S = 0;
    while ((S + 1) * (S + 2) / 2 <= n)
        ++S;

    idx_t size = S + 1;            /* table dimensions (size × size) */

    /* Allocate the table that will hold A(i,k) for 0 ≤ i,k ≤ S. */
#ifdef __cplusplus
    std::vector<seq_t> table((size_t)size * size);
#else
    seq_t *table = (seq_t *)malloc((size_t)size * size * sizeof(seq_t));
    if (!table)
        return (seq_t)0;           /* memory allocation failure */
#endif

    /* Allocate and pre‑compute f(k) for 0 ≤ k ≤ S. */
    idx_t *f = (idx_t *)malloc((size_t)size * sizeof(idx_t));
    if (!f) {
#ifdef __cplusplus
        /* nothing to free – vector will self‑destroy */
#else
        free(table);
#endif
        return (seq_t)0;
    }
    for (idx_t k = 0; k <= S; ++k) {
        idx_t m = 0;
        while ((m + 1) * (m + 2) / 2 <= k)
            ++m;
        f[k] = m;
    }

    /* Base row: A(0,k) = 1 for all k. */
    for (idx_t k = 0; k <= S; ++k) {
        table[k] = (seq_t)1;
    }

    /* Compute the rest of the table by rows. */
    for (idx_t i = 1; i <= S; ++i) {
        table[i * size] = (seq_t)0;        /* A(i,0) = 0 for i>0 */
        for (idx_t k = 1; k <= S; ++k) {
            idx_t m = f[k];
            seq_t t1 = table[i * size + (k - m)];
            seq_t t2 = ((seq_t)i + (seq_t)m - (seq_t)1) * table[(i - 1) * size + k];
            table[i * size + k] = t1 + t2;
        }
    }

    /* Convert the linear index n into coordinates (i,j) in the table. */
    idx_t prev_sum = S * (S + 1) / 2;            /* indices up to diagonal S-1 */
    idx_t j = n - prev_sum;                      /* column index on diagonal S */
    idx_t i = S - j;                             /* row index on diagonal S */

    seq_t result = table[i * size + j];

    /* Clean up temporary storage. */
    free(f);
#ifndef __cplusplus
    free(table);
#endif

    return result;
}
