/*
 * A382096  Number of rooted ordered trees with node weights summing to n,
 *          where the root has weight 0, non-root node weights are in {1,2,3},
 *          and no nodes have the same weight as their parent node.
 * https://oeis.org/A382096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G(x)=1/(1-b1*x-b2*x^2-b3*x^3) where b1(x)=1/(1-b2*x^2-b3*x^3),
 *            b2(x)=1/(1-b1*x-b3*x^3), b3(x)=1/(1-b1*x-b2*x^2). a(n) is the
 *            coefficient of x^n in G
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *b1 = NULL, *b2 = NULL, *b3 = NULL, *G = NULL;
    static idx_t capacity = 0;

    if (n + 1 > capacity) {
        idx_t newcap = n + 1;
        b1 = (seq_t *)realloc(b1, newcap * sizeof(seq_t));
        b2 = (seq_t *)realloc(b2, newcap * sizeof(seq_t));
        b3 = (seq_t *)realloc(b3, newcap * sizeof(seq_t));
        G  = (seq_t *)realloc(G,  newcap * sizeof(seq_t));
        for (idx_t i = capacity; i < newcap; ++i) {
            b1[i] = 0;
            b2[i] = 0;
            b3[i] = 0;
            G[i]  = 0;
        }
        if (capacity == 0) {
            /* base coefficients */
            b1[0] = 1;
            b2[0] = 1;
            b3[0] = 1;
            G[0]  = 1;
        }
        capacity = newcap;
    }

    for (idx_t i = (capacity == n + 1 ? 1 : capacity - 1); i <= n; ++i) {
        /* compute b1[i] */
        seq_t sum1 = 0;
        if (i >= 2) {
            for (idx_t k = 0; k <= i - 2; ++k)
                sum1 += b1[k] * b2[i - 2 - k];
        }
        if (i >= 3) {
            for (idx_t k = 0; k <= i - 3; ++k)
                sum1 += b1[k] * b3[i - 3 - k];
        }
        b1[i] = sum1;

        /* compute b2[i] */
        seq_t sum2 = 0;
        if (i >= 1) {
            for (idx_t k = 0; k <= i - 1; ++k)
                sum2 += b1[k] * b2[i - 1 - k];
        }
        if (i >= 3) {
            for (idx_t k = 0; k <= i - 3; ++k)
                sum2 += b3[k] * b2[i - 3 - k];
        }
        b2[i] = sum2;

        /* compute b3[i] */
        seq_t sum3 = 0;
        if (i >= 1) {
            for (idx_t k = 0; k <= i - 1; ++k)
                sum3 += b1[k] * b3[i - 1 - k];
        }
        if (i >= 2) {
            for (idx_t k = 0; k <= i - 2; ++k)
                sum3 += b2[k] * b3[i - 2 - k];
        }
        b3[i] = sum3;

        /* compute G[i] */
        seq_t sumG = 0;
        if (i >= 1) {
            for (idx_t k = 0; k <= i - 1; ++k)
                sumG += G[k] * b1[i - 1 - k];
        }
        if (i >= 2) {
            for (idx_t k = 0; k <= i - 2; ++k)
                sumG += G[k] * b2[i - 2 - k];
        }
        if (i >= 3) {
            for (idx_t k = 0; k <= i - 3; ++k)
                sumG += G[k] * b3[i - 3 - k];
        }
        G[i] = sumG;
    }

    return G[n];
}
