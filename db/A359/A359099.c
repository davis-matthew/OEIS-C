/*
 * A359099  a(n) = (1/6) * Sum_{d|n} phi(7 * d).
 * https://oeis.org/A359099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (1/6) * Sum_{d|n} phi(7*d)
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent multiplicative form:  let e be the exponent of 7 in n, m = n/7^e */
/* Then a(n) = m * (7^(e+1)-1)/6, i.e. multiply the part coprime to 7 by the   */
/* special factor for 7^e. This implements the integer formula directly.       */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* guard against n == 0 (offset starts at 1) */
    if (n == 0)
        return 0;

    /* Extract the exponent of 7 in n */
    idx_t temp = n;
    idx_t e = 0;
    while ((temp % 7) == 0)
    {
        temp /= 7;
        e++;
    }

    /* m is the part of n coprime to 7 */
    seq_t m = (seq_t)temp;

    /* compute 7^(e+1) in seq_t */
    seq_t pow7e1 = 1;
    for (idx_t i = 0; i < e + 1; i++)
        pow7e1 *= 7;

    /* special factor (7^(e+1)-1)/6, integer division exact */
    seq_t special = (pow7e1 - 1U) / 6U;

    return m * special;
}
