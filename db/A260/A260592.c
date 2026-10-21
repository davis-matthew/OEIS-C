/*
 * A260592  a(n) = binary odd/even encoding of the iterates in the modified
 *          Syracuse algorithm (msa) starting with 2n+1 and continuing up to
 *          (but not including) the first iterate less than 2n+1.
 * https://oeis.org/A260592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary odd/even encoding of the modified Syracuse *
 *            algorithm starting from 2n+1, returned as a decimal integer
 *            whose * digits are those bits (0 for even, 1 for odd). The
 *            sequence is * offset 1: a(1) corresponds to n=1. For n=0 we
 *            return 0 safely
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the non‑positive index that is outside the OEIS offset. */
    if (n == 0)
        return 0;

    /* Starting value 2n+1, cast to seq_t to avoid overflow. */
    seq_t base = ((seq_t)n) * ((seq_t)2) + ((seq_t)1);
    seq_t x = base;
    seq_t res = 0;      /* result accumulating decimal digits 0/1 */

    /* Iterate while the current value is not smaller than the start,
       appending parity bits as decimal digits. */
    while (x >= base)
    {
        seq_t bit = (x & (seq_t)1);            /* least significant bit */
        res = res * ((seq_t)10) + bit;         /* append as decimal digit */

        /* Next iterate of the modified Syracuse algorithm. */
        if (bit) /* x is odd */
            x = ((seq_t)3 * x + (seq_t)1) / (seq_t)2;
        else /* x is even */
            x = x / (seq_t)2;
    }

    return res;
}
