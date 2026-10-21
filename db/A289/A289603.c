/*
 * A289603  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 273).
 * https://oeis.org/A289603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = catalan(n) - 2^(n-1) - (n-3)^2 for n > 4; a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first five values directly. */
    if (n <= 4) {
        static const seq_t vals[5] = {1, 1, 1, 1, 6};
        return vals[n];
    }

    /* n >= 5: use the closed‑form expression. */
    seq_t cat   = catalan(n);          /* Catalan number C(n) */
    seq_t twop  = pow2(n - 1);          /* 2^(n-1) */
    seq_t d     = (seq_t)(n - 3);       /* (n-3) as seq_t */
    seq_t sq    = d * d;                /* (n-3)^2 */

    return cat - twop - sq;
}
