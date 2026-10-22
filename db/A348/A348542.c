/*
 * A348542  Number of partitions of n into 3 parts where at least one part is
 *          even.
 * https://oeis.org/A348542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = floor((n^2 + 3) / 12) if n is even * a(n) = floor((n^2
 *            + 3) / 12) - floor((m^2 + 6m + 12) / 12) * where m = (n - 3) / 2
 *            if n is odd * (m corresponds to the number of ways all three
 *            parts can be odd). * This follows from the partition formula for
 *            three parts * and subtracting the all‑odd...
 * Range:     u8 n <= 55, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;                /* a(1), a(2), a(3) = 0 */

    seq_t part_total = ((seq_t)n * (seq_t)n + 3) / 12;

    if ((n & 1) == 0)                  /* n is even */
        return part_total;

    /* n is odd */
    idx_t m = (n - 3) / 2;              /* (n-3)/2 is integer for odd n >=5 */
    seq_t odd_part = ((seq_t)m * (seq_t)m + 6 * (seq_t)m + 12) / 12;

    return part_total - odd_part;
}
