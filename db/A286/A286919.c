/*
 * A286919  Triangle read by rows: T(n,m) is the number of pattern classes in
 *          the (n,m)-rectangular grid with 8 colors and n>=m, two patterns
 *          are in the same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A286919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(n,m) = (8^(n*m) + …)/4 according to
 *            A286919 (triangle read by rows)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Locate the row r and column m for the flattened index n.
       Solve r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t m = n - r * (r + 1) / 2;          // 0 <= m <= r

    idx_t nm = r * m;                       // product n*m

    seq_t base8   = (seq_t)8;
    seq_t pow_nm   = big_pow(base8, (seq_t)nm);
    seq_t pow_half = big_pow(base8, (seq_t)(nm / 2));

    seq_t sum = pow_nm;

    if ((r & 1UL) == 0) {                   // r even
        if ((m & 1UL) == 0) {               // even‑even
            sum += pow_half * (seq_t)3;
        } else {                            // even, odd
            idx_t exp = (nm + r) / 2;
            seq_t pow_exp = big_pow(base8, (seq_t)exp);
            sum += pow_exp;
            sum += pow_half * (seq_t)2;
        }
    } else {                                // r odd
        if ((m & 1UL) == 0) {               // odd, even
            idx_t exp = (nm + m) / 2;
            seq_t pow_exp = big_pow(base8, (seq_t)exp);
            sum += pow_exp;
            sum += pow_half * (seq_t)2;
        } else {                            // odd, odd
            idx_t exp1 = (nm + r) / 2;
            idx_t exp2 = (nm + m) / 2;
            idx_t exp3 = (nm + 1) / 2;
            seq_t pow1 = big_pow(base8, (seq_t)exp1);
            seq_t pow2 = big_pow(base8, (seq_t)exp2);
            seq_t pow3 = big_pow(base8, (seq_t)exp3);
            sum += pow1 + pow2 + pow3;
        }
    }

    return sum / (seq_t)4;
}
