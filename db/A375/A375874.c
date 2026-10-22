/*
 * A375874  Number of distinct n X n patterns in the squiral tiling.
 * https://oeis.org/A375874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4 + 8*A - 8*B)*(n - 1)^2 + (12*3^A + 24*3^B)*(n - 1) -
 *            18*9^A for n >= 4, where A = floor(log3(n-2)), B =
 *            floor(log3((n-2)/2)) Base values: a(0)=1, a(1)=2, a(2)=14,
 *            a(3)=70
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the first four values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)70;

    /* Compute A = floor(log3(n-2)), B = floor(log3((n-2)/2)) */
    idx_t tmp;
    idx_t A = 0, B = 0;

    tmp = n - 2;
    while (tmp >= 3) { tmp /= 3; ++A; }

    tmp = (n - 2) / 2;
    while (tmp >= 3) { tmp /= 3; ++B; }

    /* Powers of 3 */
    seq_t pow3A = pow3(A);          /* 3^A      */
    seq_t pow3B = pow3(B);          /* 3^B      */
    seq_t pow9A = pow3A * pow3A;    /* 9^A = (3^A)^2 */

    seq_t n_minus1 = (seq_t)(n - 1);
    seq_t coeff = (seq_t)(4 + 8 * A - 8 * B);

    seq_t t1 = coeff * n_minus1 * n_minus1;
    seq_t t2 = ((seq_t)12 * pow3A + (seq_t)24 * pow3B) * n_minus1;
    seq_t t3 = (seq_t)18 * pow9A;

    return t1 + t2 - t3;
}
