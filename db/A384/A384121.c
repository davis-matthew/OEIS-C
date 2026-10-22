/*
 * A384121  Array read by antidiagonals: T(n,m) is the number of dominating
 *          sets in the n X m rook complement graph.
 * https://oeis.org/A384121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m)=2^(n*m)-n*(2^m-2)-m*(2^n-2)+n*m-n*m*(2^(m-1)-1)*(2^(n-1)-1)+n*(n-1)*m*(m-1)/2-1
 *            for n>1,m>1; T(n,m)=T(m,n). a(n) is read by antidiagonals of
 *            T(n,m)
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 82, u64 n <= 158,
 *            u128 n <= 285, big any n
 * Helpers:   antidiagonal_coords, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/pow2.h"

static inline seq_t T_of(idx_t i, idx_t j)
{
    if (i == 0 || j == 0 || i == 1 || j == 1) return 1;
    seq_t i_t = (seq_t)i;
    seq_t j_t = (seq_t)j;

    seq_t term = pow2((idx_t)(i * j));

    seq_t two_pow_m = pow2(j);
    term -= i_t * (two_pow_m - 2);

    seq_t two_pow_n = pow2(i);
    term -= j_t * (two_pow_n - 2);

    term += i_t * j_t;

    seq_t two_pow_m1 = pow2(j - 1);
    seq_t two_pow_n1 = pow2(i - 1);
    term -= i_t * j_t * (two_pow_m1 - 1) * (two_pow_n1 - 1);

    seq_t combo = i_t * (i_t - 1) * j_t * (j_t - 1) / 2;
    term += combo;

    term -= 1;
    return term;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return T_of(i, j);
}
