/*
 * A295910  Number of (not necessarily maximal) cliques in the n X n white
 *          bishop graph.
 * https://oeis.org/A295910
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^(n + 1) + 2^(n + 1)*(17 + (-1)^n) - 6*n*(n + 4) -
 *            23)/12
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;  /* undefined below offset */

    seq_t n_seq = (seq_t)n;
    /* (-1)^n */
    seq_t r = ((n & 1) == 0) ? (seq_t)1 : (seq_t)-1;
    /* (-1)^(n + 1) = -r */
    seq_t term1 = -r;
    /* 2^(n + 1) */
    seq_t pow2 = seq_pow((seq_t)2, n + 1);
    /* 2^(n + 1)*(17 + (-1)^n) */
    seq_t term2 = pow2 * ((seq_t)17 + r);
    /* 6*n*(n + 4) */
    seq_t term3 = (seq_t)6 * n_seq * (n_seq + (seq_t)4);
    /* numerator = term1 + term2 - term3 - 23 */
    seq_t numerator = term1 + term2 - term3 - (seq_t)23;
    /* result = numerator / 12 */
    return numerator / (seq_t)12;
}
