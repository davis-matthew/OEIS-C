/*
 * A281699  Sierpinski stellated octahedron numbers: a(n) = 2*(-3*2^(n+1) +
 *          2^(2*n+3) + 5).
 * https://oeis.org/A281699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(2^(2*n+1) + 2) - 6*(2^(n+1) + 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided pow2 function to compute 2^k. */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(2*n+1) and 2^(n+1) */
    seq_t t1 = pow2((n << 1) + 1); /* 2^(2*n+1) */
    seq_t t2 = pow2(n + 1);       /* 2^(n+1) */

    /* Apply the closed‑form expression */
    seq_t res = 8 * (t1 + 2) - 6 * (t2 + 1);
    return res;
}
