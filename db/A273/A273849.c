/*
 * A273849  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 969", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3+12*n^2+11*n-51)/3 for n>3; a(0)=1, a(1)=5,
 *            a(2)=26, a(3)=71
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* small n values */
    static const seq_t init[4] = { (seq_t)1, (seq_t)5, (seq_t)26, (seq_t)71 };
    if (n < 4)
        return init[(size_t)n];

    /* use 64‑bit arithmetic for the intermediate computation */
    idx_t sn = n;
    idx_t num = 4 * sn * sn * sn + 12 * sn * sn + 11 * sn - 51;
    return (seq_t)(num / 3);
}
