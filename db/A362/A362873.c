/*
 * A362873  a(n) is the number of points with integer coordinates that are
 *          inside an equilateral triangle inscribed in a circle of radius n,
 *          the location of the triangle in the Oxy coordinate plane is
 *          described in the comments.
 * https://oeis.org/A362873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-2)/2 + 2*Sum_{k=1..floor(sqrt(3)*n/2)}
 *            floor(-sqrt(3)*k + 3*n/2) (n even) a(n) = (3*n-1)/2 +
 *            2*Sum_{k=1..floor(sqrt(3)*n/2)} floor(-sqrt(3)*k + (3*n+1)/2) (n
 *            odd)
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* value below offset is irrelevant */

    seq_t sn = (seq_t)n;
    seq_t C, base;

    if ((n & 1ULL) == 0) {           /* n even */
        C    = ((seq_t)3 * sn) / 2;
        base = (((seq_t)3 * sn) - 2) / 2;
    } else {                         /* n odd */
        C    = (((seq_t)3 * sn) + 1) / 2;
        base = (((seq_t)3 * sn) - 1) / 2;
    }

    /* m = floor_sqrt( (3*n*n)/4 )  ≈ floor(sqrt(3)*n/2) */
    seq_t tmp = ((seq_t)3 * sn * sn) / 4;
    idx_t m = (idx_t)floor_sqrt(tmp);

    seq_t sum_s = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t prod = ((seq_t)3 * k * k);
        seq_t sk   = floor_sqrt(prod);   /* floor(sqrt(3)*k) */
        sum_s += sk;
    }

    seq_t term = ((seq_t)m * (C - (seq_t)1)) - sum_s;
    seq_t res  = base + ((seq_t)2 * term);
    return res;
}
