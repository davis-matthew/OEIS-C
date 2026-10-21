/*
 * A279512  Sierpinski octahedron numbers: a(n) = 2*6^n + 3*2^n + 1.
 * https://oeis.org/A279512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*6^n + 3*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 3^n */
    seq_t pow2n = pow2(n);          /* 2^n */
    seq_t pow3n = pow3(n);          /* 3^n */

    /* 6^n = 2^n * 3^n */
    seq_t pow6n = seq_mul(pow2n, pow3n);

    /* a(n) = 2*6^n + 3*2^n + 1 */
    seq_t res = seq_mul(pow6n, 2);  /* 2*6^n */
    res += seq_mul(pow2n, 3);       /* +3*2^n */
    res += 1;                       /* +1 */

    return res;
}
