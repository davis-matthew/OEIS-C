/*
 * A299787  Maximum size of a main class for diagonal Latin squares of order
 *          n.
 * https://oeis.org/A299787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^floor(n/2) * floor(n/2)! * 4 * n! for n>6 Special
 *            values: a(1)=1, a(2)=0, a(3)=0, a(4)=48, a(5)=480, a(6)=69120
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special cases for n <= 6 */
    static const seq_t special[6] = {
        (seq_t)1,   /* a(1) */
        (seq_t)0,   /* a(2) */
        (seq_t)0,   /* a(3) */
        (seq_t)48,  /* a(4) */
        (seq_t)480, /* a(5) */
        (seq_t)69120/* a(6) */
    };
    if (n <= 6) {
        return special[n - 1];
    }

    /* For n > 6 use the closed‑form formula */
    idx_t m = n / 2;             /* floor(n/2) */
    seq_t result = factorial(n); /* n! */
    result *= pow2(m);           /* * 2^m */
    result *= factorial(m);      /* * m! */
    result *= pow2(2);           /* * 4 = 2^2 */
    return result;
}
