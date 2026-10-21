/*
 * A268587  Expansion of x^4*(5 - 16*x + 13*x^2)/(1 - 2*x)^4.
 * https://oeis.org/A268587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^(n-7) * (n-3) * (n+4) * (n+11) / 3 Initial values:
 *            a(0)=a(1)=a(2)=a(3)=0, a(4)=5,
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 25, u64 n <= 54, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is 0 */
    if (n < 4u) {
        return 0;
    }

    /* Compute (n-3)*(n+4)*(n+11) as seq_t */
    seq_t term1 = (seq_t)(n - 3u);
    seq_t term2 = (seq_t)(n + 4u);
    seq_t term3 = (seq_t)(n + 11u);

    seq_t val = term1;
    val *= term2;
    val *= term3;

    /* Divide by 3 – this division is always exact */
    val /= 3u;

    /* Multiply or divide by a power of two */
    if (n >= 7u) {
        val *= pow2(n - 7u);          /* multiply by 2^(n-7) */
    } else {
        val /= pow2(7u - n);          /* divide by 2^(7-n) */
    }

    return val;
}
