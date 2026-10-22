/*
 * A361993  (2,1)-block array, B(2,1), of the Wythoff array (A035513), read by
 *          descending antidiagonals.
 * https://oeis.org/A361993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361993: (2,1)-block array, read by descending antidiagonals.
 *
 * Formula:  b(i,j) = F(j+1) * ( floor(2*i*φ) + floor((2*i-1)*φ) )
 *            + (4*i-3) * F(j)
 * where F is Fibonacci (A000045) and φ = (1+√5)/2.
 * The index n is mapped to the pair (i,j) by descending antidiagonals:
 *   n = total elements before diagonal S   + offset
 *   offset = n - (S-1)*S/2
 *   i = offset
 *   j = S - offset + 1
 *
 * Floor( k * φ ) is computed using the continued‑fraction convergent 987/610
 * which is less than φ but differs by <10⁻⁶ for k ≤ 300.  Since i ≤ 300
 * for all n ≤ 10 000, this gives exact floors.
 */

#include <stdbool.h>
#include "../utils/fib.h"          /* fib(idx_t n) -> seq_t */

seq_t a(idx_t n)
{
    /* Determine diagonal S such that (S-1)*S/2 < n <= S*(S+1)/2 */
    idx_t S = 1;
    while ((S * (S + 1)) / 2 < n)
        ++S;

    idx_t prev = ((S - 1) * S) / 2;
    idx_t offset = n - prev;          /* 1‑based offset inside diagonal */
    idx_t i = offset;
    idx_t j = S - offset + 1;

    /* Floor multiplication with φ approximated by 987/610 (< φ). */
    const seq_t P = 987;   /* numerator of convergent 987/610 */
    const seq_t Q = 610;   /* denominator */

    seq_t floor_2i_phi          = (seq_t)(((seq_t)(2 * i)) * P) / Q;
    seq_t floor_2i_minus1_phi   = (seq_t)(((seq_t)(2 * i - 1)) * P) / Q;
    seq_t sum_phi = floor_2i_phi + floor_2i_minus1_phi;

    seq_t Fj   = fib(j);     /* F(j)  */
    seq_t Fj1  = fib(j + 1); /* F(j+1) */

    seq_t term1 = Fj1 * sum_phi;
    seq_t term2 = (seq_t)(4 * i - 3) * Fj;

    return term1 + term2;
}
