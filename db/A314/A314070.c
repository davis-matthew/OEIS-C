/*
 * A314070  Coordination sequence Gal.6.657.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 3*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - 4*a(n-8) + 3*a(n-9) - 2*a(n-10) +
 *            2*a(n-11) - a(n-12) for n > 12. Initial values: a(0) = 1, a(1) =
 *            5, a(2) = 11, a(3) = 15, a(4) = 22, a(5) = 27, a(6) = 31, a(7) =
 *            38, a(8) = 42, a(9) = 48, a(10) = 54, a(11) = 58,...
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer to memoise computed values */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;

    /* initial 13 values */
    static const seq_t init[13] = {
        1,
        5,
        11,
        15,
        22,
        27,
        31,
        38,
        42,
        48,
        54,
        58,
        65
    };

    /* initialise on first call */
    if (seq_len == 0) {
        seq_len = 13;
        seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
        for (idx_t i = 0; i < seq_len; ++i)
            seq[i] = init[i];
    }

    /* extend and compute up to n if necessary */
    if (n + 1 > seq_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; abort execution */
            abort();
        }
        seq = tmp;

        /* compute terms from current length up to n */
        for (idx_t i = seq_len; i <= n; ++i) {
            /* recurrence uses indices i-1 ... i-12 */
            seq[i] = (2 * seq[i - 1]) - (2 * seq[i - 2]) + (3 * seq[i - 3])
                   - (4 * seq[i - 4]) + (4 * seq[i - 5]) - (4 * seq[i - 6])
                   + (4 * seq[i - 7]) - (4 * seq[i - 8]) + (3 * seq[i - 9])
                   - (2 * seq[i - 10]) + (2 * seq[i - 11]) - seq[i - 12];
        }
        seq_len = new_len;
    }

    return seq[n];
}
