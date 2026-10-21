/*
 * A261440  Array of coefficients A(n,k) of the formal power series P(n,x)
 *          read by upwards antidiagonals, where P(n,x) = Sum_{k>=0}
 *          A(n,k)*x^k = 1+x*P(n,x)^(1*n)+x^2*P(n,x)^(2*n) for n >= 0.
 * https://oeis.org/A261440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A261440:  a(n) = A(i,j) where
 *  i = d - p , j = p ,
 *  d = largest integer with d(d+1)/2 <= n,
 *  p = n - d(d+1)/2.
 *  A(i,j) is given by
 *  A(i,j) = 1/(i*j+1) * Σ_{t=0..j} (-1)^t * C(i*j+1,t) * C(2*i*j+2-2*t, j-t)
 *  (see OEIS discussion for derivation). */

#include <stdbool.h>
#include <stdint.h>

/* utility for binomial coefficients */
#include "../utils/binomial.h"

/* A(i,j) via the conjectured closed form */
static seq_t compute_A(idx_t i, idx_t j)
{
    if (j == 0u) {           /* trivial case */
        return (seq_t)1;
    }
    const idx_t ij = i * j;
    const idx_t coeff = ij + 1;          /* i*j + 1   */
    seq_t pos = 0;                      /* sum of positive terms   */
    seq_t neg = 0;                      /* sum of negative terms   */

    /* iterate over the summation index */
    for (idx_t t = 0u; t <= j; ++t)
    {
        seq_t term1 = binomial(coeff, t);                          /* C(i*j+1,t)   */
        seq_t term2 = binomial(2 * ij + 2 - 2 * t, j - t);        /* C(2*i*j+2-2*t, j-t) */
        seq_t term  = term1 * term2;                               /* product      */

        if (t & 1u) {                                            /* (-1)^t = -1   */
            neg += term;
        } else {
            pos += term;
        }
    }
    /* pos >= neg because the final result is non‑negative   */
    seq_t numer = pos - neg;                /* Σ_{t} (-1)^t * ...   */
    return numer / coeff;                   /* divide by (i*j+1)     */
}

/* a(n) as described in the OEIS entry. */
seq_t a(idx_t n)
{
    /* find the largest d with d(d+1)/2 <= n  (loop traversal is cheap) */
    idx_t d = 0u;
    while ((d + 1u) * (d + 2u) / 2u <= n) {
        ++d;
    }
    const idx_t tri = d * (d + 1u) / 2u;      /* T_d   */
    const idx_t p   = n - tri;                /* position in diagonal (0‑based) */
    const idx_t i   = d - p;                  /* row index */
    const idx_t j   = p;                      /* column index */

    return compute_A(i, j);
}
