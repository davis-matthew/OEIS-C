/*
 * A277443  Square array A(n,k) (n>=1, k>=1) read by antidiagonals: A(n,k) is
 *          the number of n-colorings of the prism graph Y_k on 2k vertices.
 * https://oeis.org/A277443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 39, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): flat index of the square array A(n,k) defined by
   A(n,k) = (n^2-3n+3)^k + (n-1)((3-n)^k + (1-n)^k) + n^2-3n+1,
   read by antidiagonals (1,1),(1,2),(2,1),(1,3),(2,2),(3,1), … */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

/* helper: exponentiation with possibly negative base */
static seq_t power_signed(int64_t baseSigned, idx_t exp)
{
    bool neg = (baseSigned < 0);
    int64_t absv = neg ? -baseSigned : baseSigned;
    seq_t powpos = pow_seq((seq_t)absv, (int)exp);
    if (exp & 1 && neg) {
        /* assume seq_t supports signed negation – works for
           arbitrary‑precision or native signed types */
        return -powpos;
    }
    return powpos;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find (i,j) such that a(n) = A(i,j) with antidiagonal ordering */
    idx_t s = 2;          /* current sum i+j */
    idx_t cum = 0;        /* cumulative count up to sum s-1 */
    while (cum + (s - 1) < n) {
        cum += s - 1;
        ++s;
    }
    idx_t i = n - cum;         /* i coordinate (1‑based) */
    idx_t j = s - i;           /* j coordinate (1‑based) */

    /* compute A(i,j) using the given formula */
    __int128 v;

    /* term1 = (i^2-3i+3)^j */
    v = (__int128)i * i - 3 * (__int128)i + 3;
    seq_t term1 = pow_seq((seq_t)v, (int)j);

    /* term2 = (i-1) * ((3-i)^j + (1-i)^j) */
    seq_t term2 = 0;
    int64_t base1 = 3 - (int64_t)i;
    int64_t base2 = 1 - (int64_t)i;
    seq_t pow1 = power_signed(base1, j);
    seq_t pow2 = power_signed(base2, j);
    term2 = (i - 1) * (pow1 + pow2);

    /* term3 = i^2-3i+1 */
    v = (__int128)i * i - 3 * (__int128)i + 1;
    seq_t term3 = (seq_t)v;

    return term1 + term2 + term3;
}
