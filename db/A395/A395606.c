/*
 * A395606  Number of involutions (elements of order <= 2) in the n-th
 *          iterated wreath product of S_3.
 * https://oeis.org/A395606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4; W(0) = 6. For n ≥ 1: W(n) = 6 * W(n-1)^3 a(n) =
 *            a(n-1)^3 + 3 * W(n-1) * a(n-1) where a(n) is the number of
 *            involutions in the n‑th iterated wreath product of S3
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* caches for values already computed */
    static seq_t *A = NULL;          /* a[ ] */
    static seq_t *W = NULL;          /* W[ ] – group orders */
    static size_t cap = 0;           /* number of cached entries */

    if (n >= (idx_t)cap) {           /* compute missing terms */
        size_t newcap = (size_t)n + 1;
        /* grow caches with proper cast for C++ */
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        W = (seq_t *)realloc(W, newcap * sizeof(seq_t));
        if (cap == 0) {                     /* init n = 0 */
            A[0] = (seq_t)4;
            W[0] = (seq_t)6;
            cap = 1;
        }
        for (size_t i = cap; i <= n; ++i) { /* compute terms i = cap .. n */
            seq_t prevA = A[i - 1];
            seq_t prevW = W[i - 1];

            /* W(i) = 6 * prevW^3 */
            seq_t w2 = prevW * prevW;
            seq_t w3 = w2 * prevW;
            W[i] = (seq_t)6 * w3;

            /* a(i) = prevA^3 + 3 * prevW * prevA */
            seq_t a2 = prevA * prevA;
            seq_t a3 = a2 * prevA;

            seq_t mult = prevW * prevA;
            mult = mult * (seq_t)3;

            A[i] = a3 + mult;
        }
        cap = newcap;
    }

    return A[(size_t)n];
}
