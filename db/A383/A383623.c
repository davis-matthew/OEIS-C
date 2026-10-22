/*
 * A383623  a(n) = 4^n - (n^2 + 3*n + 4)*2^(n-2).
 * https://oeis.org/A383623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - (n^2 + 3*n + 4)*2^(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n) {
    /* For n = 0,1 the definition gives 0. */
    if (n < 2)
        return 0;

    /* 4^n  */
    seq_t four_pow = pow4(n);

    /* 2^(n-2)  (n>=2, so n-2 fits in idx_t) */
    seq_t two_pow = pow2(n - 2);

    /* (n^2 + 3*n + 4) as seq_t */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t poly = nn + 3 * (seq_t)n + 4;

    /* (n^2 + 3*n + 4)*2^(n-2)  */
    seq_t sub = poly * two_pow;

    /* Final result  */
    return four_pow - sub;
}
