/*
 * A394939  Number of well-formed bracketed words of total length n built from
 *          the symbol * (of length 1) and two unary bracket types, with no
 *          empty bracket pair.
 * https://oeis.org/A394939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A394939
    a(0) = 1
    a(1) = 1
    for n ≥ 2  :  a(n) = a(n-1) + 2*a(n-2) +
                   2 * Σ_{i=1}^{n-3} a(i) * a(n-2-i)
    (for the recurrence use a(0)=0, a(m)=0 for m≤0;  the offset
     0 entry is returned as 1 in this implementation)
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;           /* offset entry */

    static seq_t *A = NULL;                /* a[0 .. computed]   */
    static seq_t *T = NULL;                /* convolution sums    */
    static idx_t sizeA = 0;                /* allocated size of A */
    static idx_t sizeT = 0;                /* allocated size of T */
    static idx_t computed = 0;             /* highest n computed */

    /*---------------------------------
      Allocate and initialise on first call
    ---------------------------------*/
    if (!A) {
        sizeA = 2;                         /* need indices 0..1    */
        A   = (seq_t *)malloc(sizeof(seq_t) * sizeA);
        A[0] = (seq_t)0;
        A[1] = (seq_t)1;
        computed = 1;

        sizeT = 3;                         /* indices 0..2 needed  */
        T   = (seq_t *)malloc(sizeof(seq_t) * sizeT);
        for (idx_t i = 0; i < sizeT; ++i) T[i] = (seq_t)0;
        T[2] = A[1] * A[1];                /* contribution of (1,1)*/
    }

    /*---------------------------------
      Grow arrays if a larger n is requested
    ---------------------------------*/
    if (n > computed) {
        /* extend A array to hold a[0..n]        */
        if (sizeA <= n) {
            size_t newA = n + 1;
            seq_t *tmp = (seq_t *)realloc(A, sizeof(seq_t) * newA);
            for (size_t i = sizeA; i < newA; ++i) tmp[i] = (seq_t)0;
            A = tmp;
            sizeA = newA;
        }
        /* ensure T array can hold sums up to 2*n  */
        size_t newT = 2 * n + 1;
        if (sizeT < newT) {
            seq_t *tmp = (seq_t *)realloc(T, sizeof(seq_t) * newT);
            for (size_t i = sizeT; i < newT; ++i) tmp[i] = (seq_t)0;
            T = tmp;
            sizeT = newT;
        }

        /* compute new values up to n  */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = (i >= 2) ? T[i - 2] : (seq_t)0;
            seq_t t1 = A[i - 2];
            t1 += t1;                     /* 2*a[i-2] */
            seq_t t2 = sum;
            t2 += t2;                     /* 2*sum    */
            seq_t val = A[i - 1] + t1 + t2;
            A[i] = val;

            /* update convolution sums with new index i */
            for (idx_t j = 1; j <= i; ++j) {
                idx_t k = i + j;
                seq_t prod = A[i] * A[j];
                if (i == j) {
                    T[k] += prod;          /* only once */
                } else {
                    seq_t tmp = prod + prod;
                    T[k] += tmp;            /* double for ordered pairs */
                }
            }
        }
        computed = n;
    }

    return A[n];
}
