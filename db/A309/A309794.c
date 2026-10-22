/*
 * A309794  Sum of the odd parts appearing among the second largest parts of
 *          the partitions of n into 4 parts.
 * https://oeis.org/A309794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i odd} i * (number of partitions of n into 4 parts
 *            whose * second largest part equals i). Derived from the
 *            expression in the OEIS * entry for A309794. The computation uses
 *            a closed‑form for the count of * partitions with fixed second
 *            part, which is O(1) per i
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;                    /* no 4‑part partitions */

    idx_t max_b = (n - 2) / 2;              /* largest possible second part */
    seq_t sum = 0;

    for (idx_t b = 1; b <= max_b; ++b) {
        if ((b & 1) == 0) continue;         /* only odd second parts matter */

        idx_t S = n - 2 * b - 2;            /* residual sum after fixing a+b */
        /* Count of partitions with second part = b.  Closed form: */
        idx_t u = (S < (b - 1)) ? S : (b - 1);   /* upper bound for y'  */
        idx_t p = S >> 1;                      /* floor(S/2)            */
        seq_t count;

        if (u <= p) {
            /* Sum_{y'=0..u} (y' + 1) */
            count = ((seq_t)(u + 1) * (u + 2)) / (seq_t)2;
        } else {
            /* Split the sum into two parts */
            seq_t sum1 = ((seq_t)(p + 1) * (p + 2)) / (seq_t)2;
            seq_t k = (seq_t)(u - p);
            seq_t sum2 = k * ((seq_t)(S - p + 1)) - k * (k + 1) / (seq_t)2;
            count = sum1 + sum2;
        }

        sum += count * (seq_t)b;          /* b is odd so contributes its value */
    }

    return sum;
}
