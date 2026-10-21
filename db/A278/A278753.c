/*
 * A278753  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 65", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the decimal number whose digits are the binary
 *            representation of the n-th stage of Rule 65. For even n = 2k:
 *            a(n) consists of k+1 ones a(n) = (10^(k+1) – 1) / 9 For odd n =
 *            2k+1: a(n) is k ones followed by k+2 zeros a(n) = [(10^k – 1) /
 *            9] * 10^(k+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* even n */
        idx_t m = n / 2 + 1;           /* number of ones   */
        seq_t t = pow10(m);            /* 10^m               */
        return (t - 1) / 9;            /* repunit of length m */
    } else {                           /* odd n */
        idx_t o = (n - 1) / 2;         /* number of ones   */
        idx_t z = (n + 3) / 2;         /* number of zeros  */
        if (o == 0) return 0;          /* special case n==1 */
        seq_t rep = (pow10(o) - 1) / 9;/* repunit of length o */
        return rep * pow10(z);        /* shift left by z zeros */
    }
}
