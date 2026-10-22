/*
 * A335857  a(n) is the determinant of the n X n Hankel matrix A with A(i,j) =
 *          A000108(i+j+6) for 0<=i,j<=n-1.
 * https://oeis.org/A335857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) * binomial(2n+7,3) * binomial(2n+9,7) /
 *            1260 Computed via a multiplicative approach that reduces at each
 *            step to avoid overflow
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 50, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Denominators of the product of the binomials */
    seq_t denom[4] = {120, 6, 5040, 1260};

    /* Result accumulator */
    seq_t result = 1;

    /* Build the fifteen numerator factors (with exact duplicates) */
    seq_t terms[15];
    terms[0] = (seq_t)n + 5;
    terms[1] = (seq_t)n + 4;
    terms[2] = (seq_t)n + 3;
    terms[3] = (seq_t)n + 2;
    terms[4] = (seq_t)n + 1;

    /* from C2 */
    terms[5] = ((seq_t)n * 2) + 7;
    terms[6] = ((seq_t)n * 2) + 6;
    terms[7] = ((seq_t)n * 2) + 5;

    /* from C3 */
    terms[8]  = ((seq_t)n * 2) + 9;
    terms[9]  = ((seq_t)n * 2) + 8;
    terms[10] = ((seq_t)n * 2) + 7;   /* duplicate */
    terms[11] = ((seq_t)n * 2) + 6;   /* duplicate */
    terms[12] = ((seq_t)n * 2) + 5;   /* duplicate */
    terms[13] = ((seq_t)n * 2) + 4;
    terms[14] = ((seq_t)n * 2) + 3;

    /* Multiply each term, cancelling factors with the denominators as we go */
    for (int i = 0; i < 15; ++i) {
        seq_t x = terms[i];
        for (int d = 0; d < 4; ++d) {
            if (denom[d] == 1) continue;
            seq_t g = seq_gcd(x, denom[d]);
            if (g > 1) {
                x     /= g;
                denom[d] /= g;
            }
        }
        result *= x;
    }

    /* Any remaining denominator should now be 1; divide the result if needed */
    for (int d = 0; d < 4; ++d) {
        if (denom[d] > 1) {
            result /= denom[d];
        }
    }

    return result;
}
