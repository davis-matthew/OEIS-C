/*
 * A286396  Number of inequivalent n X n matrices over GF(9) under action of
 *          dihedral group of the square D_4.
 * https://oeis.org/A286396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/8)*(9^(n^2) + 2*9^(n^2/4) + 3*9^(n^2/2) + 2*9^((n^2 +
 *            n)/2)) if n is even; a(n) = (1/8)*(9^(n^2) + 2*9^((n^2 + 3)/4) +
 *            9^((n^2 + 1)/2) + 4*9^((n^2 + n)/2)) if n is odd
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp)   */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b)       */

seq_t a(idx_t n) {
    /* Compute n^2 as idx_t – n <= 10000 guarantees no overflow */
    idx_t n_sq = n * n;

    /* Main term 9^(n^2) */
    seq_t total = big_pow((seq_t)9, n_sq);

    if ((n & 1u) == 0) {           /* n even */
        idx_t e1 = n_sq / 4;       /* n^2 / 4   */
        idx_t e2 = n_sq / 2;       /* n^2 / 2   */
        idx_t e3 = (n_sq + n) / 2; /* (n^2 + n)/2 */

        total += seq_mul(big_pow((seq_t)9, e1), (seq_t)2);
        total += seq_mul(big_pow((seq_t)9, e2), (seq_t)3);
        total += seq_mul(big_pow((seq_t)9, e3), (seq_t)2);
    } else {                       /* n odd */
        idx_t e1 = (n_sq + 3) / 4; /* (n^2 + 3)/4 */
        idx_t e2 = (n_sq + 1) / 2; /* (n^2 + 1)/2 */
        idx_t e3 = (n_sq + n) / 2; /* (n^2 + n)/2 */

        total += seq_mul(big_pow((seq_t)9, e1), (seq_t)2);
        total += seq_mul(big_pow((seq_t)9, e2), (seq_t)1);
        total += seq_mul(big_pow((seq_t)9, e3), (seq_t)4);
    }

    /* Divide by 8 – guaranteed integer */
    return total / (seq_t)8;
}
