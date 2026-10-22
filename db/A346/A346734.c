/*
 * A346734  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 * A(x)^3.
 * https://oeis.org/A346734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-4} Sum_{j=0..n-i-4} a(i) * a(j) * a(n-i-j-4);
 *            Initial values: a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence and the double convolution */
    static seq_t *A = NULL;          /* array of a(k) values          */
    static seq_t *T = NULL;          /* T[i] = sum_{p+q=i} a(p)*a(q) */
    static idx_t lenA = 0;           /* number of values in A         */
    static idx_t capA = 0;           /* capacity of A                */
    static idx_t capT = 0;           /* capacity of T                */

    /* build the sequence up to index n */
    while (lenA <= n)
    {
        seq_t a_next;

        /* compute next term ------------------------------------- */
        if (lenA < 4)
            a_next = 1;               /* a(0)..a(3) = 1              */
        else {
            idx_t k = lenA - 4;
            seq_t sum = 0;
            for (idx_t p = 0; p <= k; ++p)
                sum += A[p] * T[k - p];
            a_next = sum;
        }

        /* ensure space for A ----------------------------------- */
        if (lenA >= capA) {
            idx_t newCap = capA ? capA << 1 : 1;
            if (newCap <= lenA) newCap = lenA + 1;
            seq_t *tmp = (seq_t *)realloc(A, sizeof(seq_t) * newCap);
            if (!tmp) return 0; /* unlikely failure */
            A = tmp;
            capA = newCap;
        }
        A[lenA] = a_next;
        idx_t idx_new = lenA;        /* index of the just added element */

        /* ensure space for T ----------------------------------- */
        idx_t reqT = 2 * (lenA + 1) - 1;
        if (reqT > capT) {
            idx_t newCapT = capT ? capT << 1 : 1;
            while (newCapT < reqT) newCapT <<= 1;
            seq_t *tmpT = (seq_t *)realloc(T, sizeof(seq_t) * newCapT);
            if (!tmpT) return 0;
            if (newCapT > capT)
                memset(tmpT + capT, 0, sizeof(seq_t) * (newCapT - capT));
            T = tmpT;
            capT = newCapT;
        }

        /* update convolution T with the new element ---------------- */
        for (idx_t p = 0; p <= idx_new; ++p) {
            seq_t prod = A[p] * A[idx_new];
            idx_t sumIndex = p + idx_new;
            T[sumIndex] += prod;
            if (p != idx_new)
                T[sumIndex] += prod;           /* count symmetric pair */
        }

        ++lenA;   /* one more term computed */
    }

    return A[n];
}
