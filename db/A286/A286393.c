/*
 * A286393  Number of inequivalent n X n matrices over GF(7) under action of
 *          dihedral group of the square D_4.
 * https://oeis.org/A286393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/8)*(7^(n^2) + 2*7^(n^2/4) + 3*7^(n^2/2) +
 *            2*7^((n^2+n)/2)) if n even; (1/8)*(7^(n^2) + 2*7^((n^2+3)/4) +
 *            7^((n^2+1)/2) + 4*7^((n^2+n)/2)) if n odd
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n^2 fits comfortably in idx_t for all n up to ~10^4 (10^8). */
    idx_t n2 = n * n;

    if ((n & (idx_t)1) == 0) {            /* n even */
        seq_t t1 = seq_pow((seq_t)7, n2);
        seq_t t2 = seq_pow((seq_t)7, n2 / 4);
        seq_t t3 = seq_pow((seq_t)7, n2 / 2);
        seq_t t4 = seq_pow((seq_t)7, (n2 + n) / 2);
        return (t1 + 2 * t2 + 3 * t3 + 2 * t4) / (seq_t)8;
    } else {                               /* n odd */
        seq_t t1 = seq_pow((seq_t)7, n2);
        seq_t t2 = seq_pow((seq_t)7, (n2 + 3) / 4);
        seq_t t3 = seq_pow((seq_t)7, (n2 + 1) / 2);
        seq_t t4 = seq_pow((seq_t)7, (n2 + n) / 2);
        return (t1 + 2 * t2 + t3 + 4 * t4) / (seq_t)8;
    }
}
