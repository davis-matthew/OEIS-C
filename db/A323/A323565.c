/*
 * A323565  E.g.f. C(x) = 5 + Integral A(x)*B(x) dx such that C(x)^2 - A(x)^2
 *          = 16 and C(x)^2 - B(x)^2 = 9.
 * https://oeis.org/A323565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: The egf C(x) satisfies
 *   C'(x) = A(x)·B(x),  A'(x) = B(x)·C(x),  B'(x) = A(x)·C(x)
 *   with A(0)=3, B(0)=4, C(0)=5.
 * The coefficients a_n = C_n obey
 *   a_0 = 5,
 *   a_{n+1} = Σ_{k=0}^{n} binom(n,k) · A_k · B_{n-k},
 *   where A_k and B_k are the coefficients of A(x) and B(x)
 *   and are obtained by the analogous recurrences.
 * The following code computes a_n for any n≥0 using the
 * convolution identities derived from the differential system.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Helper: multiply two seq_t values using 128‑bit intermediates when
 * seq_t is a built‑in unsigned integer (q = uint64_t).  For an
 * arbitrary‑precision seq_t type this macro simply performs the
 * ordinary multiplication (operator*).  The macro expands to a
 * value of type seq_t and can be used wherever a multiplication
 * occurs. */
#if defined(__SIZEOF_INT128__) && \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
/* We assume seq_t is one of the standard unsigned integer types. */
static inline seq_t mul_seq(seq_t a, seq_t b) {
    return (seq_t)(((__uint128_t)a) * b);
}
#else
/* Fall back to normal multiplication (works for arbitrary‑precision types). */
static inline seq_t mul_seq(seq_t a, seq_t b) {
    return a * b;
}
#endif

/* Dynamic allocation helpers that are portable to both C and C++ */
#ifdef __cplusplus
#  define ALLOC_SEQ_ARRAY(n)    new seq_t[(n)]
#  define FREE_SEQ_ARRAY(p)     delete[] (p)
#else
#  define ALLOC_SEQ_ARRAY(n)    ((seq_t*)calloc((size_t)(n), sizeof(seq_t)))
#  define FREE_SEQ_ARRAY(p)     free(p)
#endif

seq_t a(idx_t n)
{
    /* a(0) = 5 is the constant term of C(x). */
    if (n == 0)
        return (seq_t)5;

    /* Arrays to hold coefficients of A, B, and C up to index n. */
    seq_t *A = ALLOC_SEQ_ARRAY(n + 1);
    seq_t *B = ALLOC_SEQ_ARRAY(n + 1);
    seq_t *C = ALLOC_SEQ_ARRAY(n + 1);
    if (!A || !B || !C)
        return (seq_t)0;               /* allocation failure → 0 */

    /* Initial values (index 0). */
    A[0] = (seq_t)3;
    B[0] = (seq_t)4;
    C[0] = (seq_t)5;

    /* Compute successive coefficients. */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sumA = 0, sumB = 0, sumC = 0;
        seq_t comb  = 1;               /* binom(m‑1,0) */
        idx_t lim   = m - 1;

        for (idx_t k = 0; k <= lim; ++k) {
            seq_t bk = B[k];
            seq_t ck = C[lim - k];
            seq_t ak = A[k];

            /* Products for the convolutions. */
            seq_t prodA = mul_seq(bk, ck);        /* B_k · C_{m‑1‑k} */
            seq_t prodB = mul_seq(ak, ck);        /* A_k · C_{m‑1‑k} */
            seq_t prodC = mul_seq(ak, B[lim - k]);/* A_k · B_{m‑1‑k} */

            /* Weighted addition using current binomial coefficient. */
            sumA += mul_seq(comb, prodA);
            sumB += mul_seq(comb, prodB);
            sumC += mul_seq(comb, prodC);

            /* Update binomial coefficient for next k. */
            if (k < lim) {
                comb = mul_seq(comb, (seq_t)(lim - k));
                comb = comb / (seq_t)(k + 1);   /* exact division */
            }
        }

        A[m] = sumA;
        B[m] = sumB;
        C[m] = sumC;
    }

    seq_t result = C[n];

    /* Clean up. */
    FREE_SEQ_ARRAY(A);
    FREE_SEQ_ARRAY(B);
    FREE_SEQ_ARRAY(C);

    return result;
}
