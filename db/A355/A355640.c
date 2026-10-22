/*
 * A355640  a(0) = 0, and for any n > 0, a(n) is the least positive multiple
 *          of n whose balanced ternary expansion contains as many negative
 *          trits as positive trits.
 * https://oeis.org/A355640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * A355639(n), where A355639(n) is the smallest positive
 *            integer k such that the balanced‑ternary expansion of k*n
 *            contains an equal number of +1 and –1 trits
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   balanced_zero_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/balanced_zero_digit_sum.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == (idx_t)0)
        return (seq_t)0;

    const seq_t n_seq = (seq_t)n;   /* convert index to seq_t once */
    seq_t m = n_seq;                /* first multiple: k = 1 */

    while (1)
    {
        if (balanced_zero_digit_sum(m))
            return m;               /* found the least balanced multiple */

        m += n_seq;                 /* next multiple of n */
    }
}
