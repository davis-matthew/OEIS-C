/*
 * A391483  Number of labeled commutative magmas with n elements and no
 *          idempotents.
 * https://oeis.org/A391483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n*(n-1)/2)*(n-1)^n
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0u) {
        return (seq_t)1;
    }
    seq_t n_seq = (seq_t)n;
    seq_t exp1 = n_seq * ((seq_t)(n - 1)) / 2;   /* n*(n-1)/2 */
    seq_t term1 = big_pow(n_seq, exp1);          /* n^(n*(n-1)/2) */
    seq_t term2 = big_pow((seq_t)(n - 1), n_seq);/* (n-1)^n */
    return term1 * term2;
}
