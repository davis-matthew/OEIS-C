/*
 * A372212  a(n) = [x^n] f(x)^n, where f(x) = (1 - x^7)^7/((1 - x^2)^2 * (1 -
 *          x^5)^5).
 * https://oeis.org/A372212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   compute_binom_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A372212
    a(n) = [x^n] ((1 - x^7)^7 / ((1 - x^2)^2 * (1 - x^5)^5))^n
    For n = 0, a(0) = 1.
    This implementation evaluates the coefficient
      a(n) = Σ_{i,j,k ≥ 0, 7i+2j+5k = n}
             (-1)^i * C(7n,i) * C(2n + j - 1 , j) * C(5n + k - 1 , k)
    Using simple multiplicative formulas to generate all binomial
    coefficients.  All arithmetic is performed with the generic
    integer type seq_t; the function works for arbitrary-precision,
    64‑bit, or other seq_t definitions provided by the build system.
*/

#include <stdlib.h>  /* malloc, free */
#include "../utils/compute_binom_seq.h"

/* Return the coefficient of x^n in f(x)^n for the generating function
   f(x) = (1 - x^7)^7 / ( (1 - x^2)^2 * (1 - x^5)^5 ).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Pre‑computations for binomial coefficients. */
    idx_t max_i = n / 7;          /* i ≤ 7n, but only i ≤ n/7 can appear */
    idx_t max_j = n / 2;          /* j ≤ (n - 7i)/2 ≤ n/2 */
    idx_t max_k = n / 5;          /* k ≤ (n - 7i)/5 ≤ n/5 */

    /* Allocate arrays for the three families of binomials. */
    size_t szC7   = (size_t)(max_i   + 1);
    size_t szC2   = (size_t)(max_j   + 1);
    size_t szC5   = (size_t)(max_k   + 1);

    seq_t *C7 = (seq_t*)malloc(szC7 * sizeof(seq_t));
    seq_t *C2 = (seq_t*)malloc(szC2 * sizeof(seq_t));
    seq_t *C5 = (seq_t*)malloc(szC5 * sizeof(seq_t));

    if (!C7 || !C2 || !C5) { free(C7); free(C2); free(C5); return 0; }

    /* Compute C(7n, i) for i = 0..max_i. */
    C7[0] = (seq_t)1;
    seq_t n7 = (seq_t)7 * n;  /* 7n as seq_t */
    for (idx_t i = 1; i <= max_i; ++i) {
        /* C(7n, i) = C(7n, i-1) * (7n - (i-1)) / i */
        C7[i] = (C7[i-1] * (n7 - (i-1))) / i;
    }

    /* Compute C(2n + j - 1, j) for j = 0..max_j. */
    C2[0] = (seq_t)1;
    seq_t two_n = (seq_t)2 * n;
    compute_binom_seq(C2, max_j, two_n);

    /* Compute C(5n + k - 1, k) for k = 0..max_k. */
    C5[0] = (seq_t)1;
    seq_t five_n = (seq_t)5 * n;
    compute_binom_seq(C5, max_k, five_n);

    /* Main summation over i and k. */
    seq_t result = 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t rem = n - 7 * i;            /* remaining exponent to be split */
        if (rem > max_j*2 + max_k*5) continue;  /* safety, should not happen */

        /* For a given i, iterate over k such that 5k ≤ rem. */
        idx_t max_k_for_i = rem / 5;
        if (max_k_for_i > max_k) max_k_for_i = max_k;

        for (idx_t k = 0; k <= max_k_for_i; ++k) {
            idx_t rem2 = rem - 5 * k;
            if (rem2 & 1u) continue;      /* need rem2 even for j integer */
            idx_t j = rem2 >> 1;          /* j = (rem2)/2 */

            seq_t term = C7[i] * C2[j] * C5[k];
            if (i & 1u) { /* odd i → negative sign */
                result -= term;
            } else {
                result += term;
            }
        }
    }

    free(C7);
    free(C2);
    free(C5);
    return result;
}
