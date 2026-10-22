/*
 * A316505  a(n) is the smallest number k > 1 such that k^n - 1 is divisible
 *          by 3^n.
 * https://oeis.org/A316505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n - A007949(n)) - (-1)^n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   _v3_of, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A007949(n) is the exponent of 3 dividing n.             */
/* The function uses the provided pow3(idx_t exp) to compute 3^exp. */

#include "../utils/pow3.h"
#include "../utils/_v3_of.h"

seq_t a(idx_t n) {
    /* n ≥ 1 */
    idx_t v3 = _v3_of(n);            /* v3 = A007949(n) */
    idx_t exp = n - v3;              /* exponent for 3^exp */
    seq_t val = pow3(exp);           /* 3^(n - v3) */

    /* Subtract (-1)^n: if n even, subtract 1; if n odd, add 1. */
    if ((n & 1u) == 0) {
        val -= 1;
    } else {
        val += 1;
    }
    return val;
}
