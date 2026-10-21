/*
 * A267465  Number of length-n 0..2 arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 3^(n-1) + 2^(n-2)*(n+1) - 1 (n >= 2) with a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 3;
    }

    seq_t p2n  = pow2(n);            /* 2^n */
    seq_t p3n1 = pow3(n - 1);        /* 3^(n-1) */
    seq_t p2n2 = pow2(n - 2);        /* 2^(n-2) */

    seq_t part = p2n2 * ((seq_t)(n + 1));
    seq_t res  = p2n + p3n1 + part - 1;

    return res;
}
