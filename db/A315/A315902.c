/*
 * A315902  Coordination sequence Gal.6.282.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + a(n-8) - a(n-12) for n > 14 Initial values:
 *            a(0)=1, a(1)=6, a(2)=8, a(3)=14, a(4)=18, a(5)=26, a(6)=26,
 *            a(7)=34, a(8)=36, a(9)=50, a(10)=42, a(11)=58, a(12)=54,
 *            a(13)=70, a(14)=56
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static const seq_t base[15] = {
        (seq_t)1, (seq_t)6, (seq_t)8, (seq_t)14, (seq_t)18,
        (seq_t)26, (seq_t)26, (seq_t)34, (seq_t)36, (seq_t)50,
        (seq_t)42, (seq_t)58, (seq_t)54, (seq_t)70, (seq_t)56
    };

    if (n < 15)
        return base[(size_t)n];

    /* Allocate array to hold values up to n */
    size_t len = (size_t)n + 1;
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
    if (!seq)
        exit(EXIT_FAILURE);

    /* Copy base values */
    for (size_t i = 0; i < 15; ++i)
        seq[i] = base[i];

    /* Compute remaining terms using the recurrence */
    for (size_t i = 15; i <= (size_t)n; ++i)
        seq[i] = seq[i - 4] + seq[i - 8] - seq[i - 12];

    seq_t result = seq[(size_t)n];
    free(seq);
    return result;
}
