/*
 * A327104  Maximum vertex-degree of the set-system with BII-number n.
 * https://oeis.org/A327104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = maximum vertex-degree of the set-system with BII-number n.
   Computed by iterating over the binary indices of n and the binary
   indices of each such index. */
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t deg[65] = {0};

    /* iterate over set bits of n; positions are counted from 1 */
    for (idx_t pos = 1; pos <= 64; ++pos) {
        if ((n >> (pos - 1)) & 1) {
            idx_t i = pos;                 /* i is the integer whose binary indices form an edge */
            for (idx_t j = 1; (i >> (j - 1)); ++j) {
                if ((i >> (j - 1)) & 1) {
                    deg[(size_t)j]++;      /* vertex j appears in this edge */
                }
            }
        }
    }

    seq_t max_deg = 0;
    for (size_t j = 1; j <= 64; ++j) {
        if (deg[j] > max_deg) {
            max_deg = deg[j];
        }
    }
    return max_deg;
}
