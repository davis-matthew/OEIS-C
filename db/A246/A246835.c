/*
 * A246835  Expansion of psi(-x)^2 * phi(x^2) in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A246835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 878, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_and_zero_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A246835
 *  a(n) = coefficient of x^n in psi(-x)^2 * phi(x^2)
 *  where psi(q) = Σ_{k≥0} q^{k(k+1)/2}  and  phi(q) = Σ_{k∈ℤ} q^{k^2}.
 *  Using ψ(-x)=Σ (-1)^{k(k+1)/2} x^{k(k+1)/2} and φ(x^2)=1+2Σ_{t≥1}x^{2t^2}
 *  we obtain
 *
 *      a(n) = Σ_{k,l} (-1)^{k(k+1)/2}(-1)^{l(l+1)/2}  
 *               · [k(k+1)/2 + l(l+1)/2 = n]
 *        + 2 · Σ_{t≥1} Σ_{k,l} (-1)^{k(k+1)/2}(-1)^{l(l+1)/2}  
 *               · [k(k+1)/2 + l(l+1)/2 = n-2t^2]
 *
 *  The following implementation computes the two summations by
 *  convolution of the sparse ψ(-x) series and then the φ(x^2) shift.
 *  Complexity is O(K^2 + √n) per call, with K ≈ √(2n) the number
 *  of triangular numbers ≤ n.  It works for any integer type
 *  seq_t and idx_t supplied by the build system.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_and_zero_size_t.h"

/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Convert index to size_t for loop control and array size */
    size_t N = (size_t)n;

    /* Find how many triangular numbers are ≤ N */
    size_t K = 0;
    for (size_t k = 0;; ++k) {
        size_t tri = k * (k + 1) / 2;
        if (tri > N) break;
        ++K;
    }

    if (K == 0) {   /* Only N == 0 is possible here */
        return (seq_t)1;
    }

    /* Allocate arrays for triangular numbers and their signs */
    size_t *tri = (size_t *)malloc(K * sizeof(size_t));
    signed char *sgn = (signed char *)malloc(K * sizeof(signed char));

    for (size_t k = 0; k < K; ++k) {
        tri[k] = k * (k + 1) / 2;
        /* sign = (-1)^{k(k+1)/2} → +1 for k%4 in {0,3}, -1 otherwise */
        sgn[k] = ((k & 3) == 0 || (k & 3) == 3) ? 1 : -1;
    }

    /* Allocate convolution array conv[0..N] and zero it */
    seq_t *conv = alloc_and_zero_size_t(N + 1);

    /* Compute the convolution ψ(-x)^2 */
    for (size_t i = 0; i < K; ++i) {
        for (size_t j = 0; j < K; ++j) {
            size_t sum = tri[i] + tri[j];
            if (sum > N) continue;
            conv[sum] += (seq_t)sgn[i] * (seq_t)sgn[j];
        }
    }

    /* Assemble the final coefficient a(N) */
    seq_t result = conv[N];
    for (size_t t = 1;; ++t) {
        size_t sub = 2 * t * t;
        if (sub > N) break;
        result += (seq_t)2 * conv[N - sub];
    }

    /* Free temporary memory */
    free(conv);
    free(tri);
    free(sgn);

    return result;
}
