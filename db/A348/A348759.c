/*
 * A348759  (43200/719)*{a(n)} are the times, measured in seconds from
 *          00:00:00, at which the angle of the sector enclosing the three
 *          hands of an analog clock has a local minimum.
 * https://oeis.org/A348759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = round((n-1)*719/11) (offset 1)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* behaviour below the offset does not matter */
        return (seq_t)0;

    /* compute (n-1)*719 + 5  then divide by 11 */
    seq_t prod = ((seq_t)(n - 1) * (seq_t)719) + (seq_t)5;
    return prod / (seq_t)11;
}
