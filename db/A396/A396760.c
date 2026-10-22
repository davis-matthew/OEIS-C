/*
 * A396760  Positive integers k at which A122921(k) - ceiling(sqrt(k/4))
 *          reaches a new record value.
 * https://oeis.org/A396760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=11, a(3)=53. For n>=4, a(n) is the (n-3)-rd
 *            smallest value among the three geometric sequences {96·4^m},
 *            {224·4^m}, and {2816·4^m} (m>=0). The terms are merged in
 *            increasing order
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three special terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)53;

    /* Number of terms needed from the merged chains */
    idx_t target = n - 4;        /* 0-based index among the merged values */

    /* Current values of the three chains, initialized to the m=0 terms */
    seq_t val[3] = { (seq_t)96, (seq_t)224, (seq_t)2816 };

    seq_t result = 0;
    idx_t   pos   = 0;

    /* Generate terms until the target index is reached */
    while (pos <= target) {
        /* Find the index of the smallest current value */
        idx_t minidx = 0;
        if (val[1] < val[minidx]) minidx = 1;
        if (val[2] < val[minidx]) minidx = 2;

        /* This is the next output term */
        result = val[minidx];

        /* Advance the chain that produced this term */
        val[minidx] = val[minidx] * (seq_t)4;

        pos++;
    }

    return result;
}
