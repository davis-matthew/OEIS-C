/*
 * A271923  Numerator of (1/3)*(Product_{j=0..n-1}
 *          (((2*j+1)*(3*j+4))/((j+1)*(6*j+1))) - 1).
 * https://oeis.org/A271923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (1/3)*(Product_{j=0..n-1}
 *            ((2*j+1)*(3*j+4))/((j+1)*(6*j+1)) - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t prod_num = 1;
    seq_t prod_den = 1;

    for (idx_t j = 0; j < n; ++j) {
        /* Compute term numerator and denominator */
        seq_t term_num = ((seq_t)2 * j + 1) * ((seq_t)3 * j + 4);
        seq_t term_den = (j + 1) * ((seq_t)6 * j + 1);

        /* Cross‑cancel with existing numerator/denominator to keep numbers small */
        seq_t g1 = gcd(term_num, prod_den);
        term_num /= g1;
        prod_den /= g1;

        seq_t g2 = gcd(term_den, prod_num);
        term_den /= g2;
        prod_num /= g2;

        /* Multiply with 128‑bit intermediate to avoid overflow */
        prod_num = (seq_t)((__int128)prod_num * term_num);
        prod_den = (seq_t)((__int128)prod_den * term_den);

        /* Reduce any new common factors */
        seq_t g3 = gcd(prod_num, prod_den);
        prod_num /= g3;
        prod_den /= g3;
    }

    /* Compute (P - 1) = (prod_num - prod_den) / prod_den */
    seq_t diff_num = prod_num - prod_den;      /* numerator of P - 1 */
    seq_t diff_den = prod_den;                 /* denominator of P - 1 */

    /* Compute (1/3)*(P - 1) = diff_num / (3*diff_den) and reduce */
    seq_t three_den = (seq_t)((__int128)3 * diff_den);
    seq_t g4 = gcd(diff_num, three_den);
    seq_t result = diff_num / g4; /* this is the required numerator */

    return result;
}
