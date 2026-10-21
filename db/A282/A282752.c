/*
 * A282752  Coefficients in q-expansion of E_2^2*E_4^2, where E_2 and E_4 are
 *          respectively the Eisenstein series A006352 and A004009.
 * https://oeis.org/A282752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of qⁿ in (E₂(q))²·(E₄(q))² where E₂(q) = 1 –
 *            24 ∑ₖ₌₁^∞ σ₁(k) qᵏ and E₄(q) = 1 + 240 ∑ₖ₌₁^∞ σ₃(k) qᵏ σ₁ and σ₃
 *            are the divisor–sum functions
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 5, i64 n <= 35, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#define ALLOCARRAY(T, N)  new T[(N)]
#define FREEARRAY(T, P)   delete[] (P)
#else
#define ALLOCARRAY(T, N)  ((T*)calloc((N), sizeof(T)))
#define FREEARRAY(T, P)   free(P)
#endif

/* Compute a(n) directly (O(n²) time, O(n) memory) */
seq_t a(idx_t n)
{
    /* All work is done in the function body; no global state is kept */
    size_t N = (size_t)n;
    if (N > N) {   /* just to silence unused warning in some compilers */
        return (seq_t)0;
    }

    /* Allocate arrays of size N+1, initialized to zero */
    seq_t *sigma1 = ALLOCARRAY(seq_t, N + 1);
    seq_t *sigma3 = ALLOCARRAY(seq_t, N + 1);
    seq_t *f      = ALLOCARRAY(seq_t, N + 1);
    seq_t *a_arr  = ALLOCARRAY(seq_t, N + 1);
    if (!sigma1 || !sigma3 || !f || !a_arr) {
        /* Allocation failed – abort gracefully */
        exit(1);
    }

    /* 1. Compute σ₁ and σ₃ for all m ≤ N */
    for (size_t d = 1; d <= N; ++d) {
        seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;  /* (d)^3 */
        for (size_t m = d; m <= N; m += d) {
            sigma1[m] += (seq_t)d;
            sigma3[m] += d3;
        }
    }

    /* 2. Compute coefficients of E₂·E₄ :
       f[0] = 1
       f[n] = –24 σ₁(n) + 240 σ₃(n) – 5760 ∑_{k=1}^{n-1} σ₁(k)·σ₃(n–k)  */
    const seq_t C24  = (seq_t)-24;
    const seq_t C240 = (seq_t)240;
    const seq_t C5760 = (seq_t)-5760;   /* –24 * 240 */

    f[0] = (seq_t)1;
    for (size_t n_i = 1; n_i <= N; ++n_i) {
        seq_t sum = C24 * sigma1[n_i] + C240 * sigma3[n_i];
        for (size_t k = 1; k < n_i; ++k) {
            sum += C5760 * sigma1[k] * sigma3[n_i - k];
        }
        f[n_i] = sum;
    }

    /* 3. Convolution to get a[n] = (E₂·E₄)² coefficient */
    a_arr[0] = f[0] * f[0];
    for (size_t n_i = 1; n_i <= N; ++n_i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= n_i; ++k) {
            sum += f[k] * f[n_i - k];
        }
        a_arr[n_i] = sum;
    }

    seq_t result = a_arr[N];

    /* Free temporary arrays */
    FREEARRAY(seq_t, sigma1);
    FREEARRAY(seq_t, sigma3);
    FREEARRAY(seq_t, f);
    FREEARRAY(seq_t, a_arr);

    return result;
}
