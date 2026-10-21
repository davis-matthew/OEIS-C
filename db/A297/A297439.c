/*
 * A297439  Number of maximum independent vertex sets and minimum vertex
 *          covers in the n-web graph.
 * https://oeis.org/A297439
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 2^(n+1) a(2n+1) = 3 * 2^(n-1) * (2n + 1) The sequence is
 *            defined for n ≥ 2; values for n < 2 are undefined. This
 *            implementation uses the provided pow2 function for
 *            exponentiation
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* Undefined for n < 2 – return 0 as a safe placeholder */

    if ((n & 1) == 0) {                 /* n even: n = 2k */
        idx_t k = n >> 1;               /* k = n/2 */
        return pow2(k + 1);
    } else {                            /* n odd: n = 2k + 1 */
        idx_t k = (n - 1) >> 1;         /* k = (n-1)/2 */
        /* Compute 3 * 2^(k-1) * (2k + 1) */
        seq_t val = pow2(k < 1 ? 0 : k - 1);   /* 2^(k-1), k≥1 for n≥3 */
        val *= (seq_t)3;
        val *= (seq_t)(2 * k + 1);
        return val;
    }
}
