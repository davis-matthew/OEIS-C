/*
 * A245940  a(n) = (2n^7 + 4n^6 - n^5 - 4n^4 - n^3) / 24.
 * https://oeis.org/A245940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n - 1)*(n + 1)*(2*n^2 + 4*n + 1)/24
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 805,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle small indices directly to avoid underflow */
    if (n <= 1)
        return 0;

    seq_t tn = (seq_t)n;
    seq_t tn_minus1 = tn - 1;   /* n-1 */
    seq_t tn_plus1  = tn + 1;   /* n+1 */

    /* Compute 2*n^2 + 4*n + 1 */
    seq_t d = 2 * tn * tn + 4 * tn + 1;

    /* Factors for the product: n, n, n, (n-1), (n+1), d */
    seq_t factors[6] = { tn, tn, tn, tn_minus1, tn_plus1, d };

    seq_t divisor = 24;   /* divide by 24 at the end */
    seq_t result  = 1;

    for (int i = 0; i < 6; ++i) {
        seq_t g = gcd(factors[i], divisor);
        if (g > 1) {
            factors[i] /= g;
            divisor   /= g;
        }
        result *= factors[i];
    }

    /* In the (unlikely) case divisor > 1, finish the division */
    if (divisor > 1)
        result /= divisor;

    return result;
}
