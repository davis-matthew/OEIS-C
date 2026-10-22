/*
 * A371943  Number of permutations that end with a consecutive pattern 123,
 *          and avoid consecutive patterns 123 and 213 elsewhere.
 * https://oeis.org/A371943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2) + b(n-1), b(n) = b(n-1) +
 *            (n-1)*b(n-2) Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1,
 *            a(4)=2 b(0)=0, b(1)=0, b(2)=0, b(3)=1, b(4)=4 Computation is
 *            memoised and works for any n >= 0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *A = NULL;       /* a values        */
    static seq_t *B = NULL;       /* b values        */
    static idx_t capacity = 0;    /* current allocation size */
    static idx_t Nmax = 0;        /* highest index computed */

    /* initialise tables with base values */
    if (A == NULL) {
        capacity = 5;                /* store indices 0..4 */
        A = (seq_t*)malloc(capacity * sizeof(seq_t));
        B = (seq_t*)malloc(capacity * sizeof(seq_t));
        /* a sequence base values */
        A[0] = (seq_t)0; A[1] = (seq_t)0; A[2] = (seq_t)0;
        A[3] = (seq_t)1; A[4] = (seq_t)2;
        /* b sequence base values */
        B[0] = (seq_t)0; B[1] = (seq_t)0; B[2] = (seq_t)0;
        B[3] = (seq_t)1; B[4] = (seq_t)4;
        Nmax = 4;          /* last computed index        */
    }

    /* if the required index is already computed, return it */
    if (n <= Nmax) {
        return A[n];
    }

    /* ensure sufficient capacity, grow by factor 2 */
    if (n >= capacity) {
        idx_t newcap = capacity;
        while (newcap <= n) newcap <<= 1;
        A = (seq_t*)realloc(A, newcap * sizeof(seq_t));
        B = (seq_t*)realloc(B, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* compute subsequent values up to n */
    for (idx_t i = Nmax + 1; i <= n; ++i) {
        seq_t bi = B[i-1] + (seq_t)(i-1) * B[i-2];
        seq_t ai = A[i-1] + (seq_t)(i-1) * A[i-2] + B[i-1];
        B[i] = bi;
        A[i] = ai;
    }
    Nmax = n;
    return A[n];
}
