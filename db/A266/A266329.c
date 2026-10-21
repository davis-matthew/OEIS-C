/*
 * A266329  E.g.f. A(x) satisfies: A(x) = exp( Integral B(x) dx ) such that
 *          B(x) = exp(x) * exp( Integral A(x) dx ), where the constant of
 *          integration is zero.
 * https://oeis.org/A266329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A266329
 *  Derived from the functional equations
 *      A(x) = exp( ∫_0^x B(t) dt ),
 *      B(x) = exp(x) * exp( ∫_0^x A(t) dt ).
 *
 *  Let A(x)=∑_{n≥0} a_n x^n/n! ,  B(x)=∑_{n≥0} b_n x^n/n!.
 *  Introducing 
 *      G(x)=∫_0^x A(t) dt = ∑_{n≥1} a_{n-1} x^n/n!,
 *      H(x)=∫_0^x B(t) dt = ∑_{n≥1} b_{n-1} x^n/n!,
 *
 *  The compositions B=exp(x+G) and A=exp(H) lead to the
 *  following recurrences (a_0=b_0=1):
 *
 *      a_n = Σ_{k=0}^{n-1} C(n-1,k) * a_k * b_{n-1-k}
 *      b_n = Σ_{k=0}^{n-1} C(n-1,k) * b_k * y_{n-k}
 *            where y_1 = 2,  y_m = a_{m-1}  (m≥2)
 *
 *  The binomial C(n-1,k) is computed incrementally, so no
 *  large factorials are formed.  All arithmetic is performed
 *  with type seq_t, which may be an arbitrary-precision integer.
 */

#include <stdlib.h>
#include <stdint.h>

/* public entry point */
seq_t a(idx_t n)
{
    static seq_t *A = NULL;   /* a array  */
    static seq_t *B = NULL;   /* b array  */
    static size_t cap = 0;    /* allocated capacity */
    static size_t len = 0;    /* number of computed terms   */

    if (n < len)
        return A[(size_t)n];

    /* grow storage to accommodate n        */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = (cap == 0 ? 1 : cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        A = tmp;
        tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        B = tmp;
        cap = new_cap;
    }

    /* initialise first term if table empty   */
    if (len == 0) {
        A[0] = (seq_t)1;
        B[0] = (seq_t)1;
        len = 1;
    }

    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* ---------- compute a(i) ---------- */
        seq_t sum_a = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binom * A[(size_t)k];
            term = term * B[(size_t)(i - 1 - k)];
            sum_a += term;
            if (k + 1 < i) {
                /* C(i-1,k+1) = C(i-1,k) * (i-1-k)/(k+1) */
                binom = binom * (seq_t)(i - 1 - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        A[(size_t)i] = sum_a;

        /* ---------- compute b(i) ---------- */
        seq_t sum_b = (seq_t)0;
        binom = (seq_t)1;                    /* C(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t y = (idx_t)(i - k == 1) ? (seq_t)2 : A[(size_t)(i - k - 1)];
            seq_t term = binom * B[(size_t)k];
            term = term * y;
            sum_b += term;
            if (k + 1 < i) {
                /* C(i-1,k+1) = C(i-1,k) * (i-1-k)/(k+1) */
                binom = binom * (seq_t)(i - 1 - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        B[(size_t)i] = sum_b;
    }

    len = n + 1;
    return A[(size_t)n];
}
