/*
 * A372088  G.f. A(x) satisfies A(x) = 1/( 1 - 4*x*(1 + x*A(x)) )^(1/2).
 * https://oeis.org/A372088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(n/2+k/2-1/2,k) * * *
 *            binomial(k,n-k)/(n-k+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static caches for a(n), the convolution S(n)=∑a(i)a(n-i),
 * and the convolution T(n)=∑a(i)S(n-i) (=coefficients of A^3) */
static seq_t *A = NULL;   /* a(n)  */
static seq_t *S = NULL;   /* convolution of a with a          */
static seq_t *T = NULL;   /* convolution of a with S (i.e. A^3) */
static size_t  cap   = 0; /* allocated size of the caches      */
static idx_t   computed = 0; /* highest index that has been computed */

/* Ensure that the caches can hold indices up to n */
static void grow_cache(idx_t n)
{
    if (n < cap) return;
    size_t new_cap = (cap == 0) ? 1 : cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
    T = (seq_t *)realloc(T, new_cap * sizeof(seq_t));
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!A) { /* first call – initialise the caches */
        grow_cache(0);
        A[0] = 1;          /* a(0) = 1 */
        S[0] = 1;          /* S(0) = a0*a0 = 1 */
        T[0] = 1;          /* T(0) = a0*S0  = 1 */
        computed = 0;
    }

    if (n > computed) {
        grow_cache(n);

        /* Compute terms from computed+1 up to n */
        for (idx_t k = computed + 1; k <= n; ++k) {
            /* sum_{i=1}^{k-1} a(i)*a(k-i)  */
            seq_t sum12 = 0;
            for (idx_t i = 1; i < k; ++i) {
                sum12 += A[i] * A[k - i];
            }

            /* S(k) = 4 * S(k-1) + 4 * T(k-2) (with T(-1)=0 for k<2) */
            seq_t Sk = S[k - 1] * 4;
            if (k >= 2) Sk += T[k - 2] * 4;

            /* a(k) = (S(k) - sum12) / 2  */
            seq_t ak = (Sk - sum12) / 2;
            A[k] = ak;
            S[k] = Sk;

            /* T(k) = sum_{i=0}^{k} A[i] * S(k-i)   */
            seq_t Tk = 0;
            for (idx_t i = 0; i <= k; ++i) {
                Tk += A[i] * S[k - i];
            }
            T[k] = Tk;
        }

        computed = n;
    }

    return A[n];
}
