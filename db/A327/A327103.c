/*
 * A327103  Minimum vertex-degree in the set-system with BII-number n.
 * https://oeis.org/A327103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum vertex-degree in the set-system with BII-number n.
   The set-system is formed from the binary indices of n:
   for each bit position i of n, the binary indices of i form an edge.
   a(n) = the minimum degree among all vertices that appear in the system. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* no vertices when n = 0 */
        return 0;

    /* The vertex labels never exceed 7 (the binary length of 64).
       Allocate a fixed-size buffer large enough for all possible indices. */
    seq_t deg[65]   = {0};
    bool  present[65] = {false};

    idx_t tmp = n;
    idx_t bitpos = 1;                 /* bit positions are 1‑based */
    while (tmp) {
        if (tmp & 1ULL) {             /* bit at position 'bitpos' is set */
            idx_t i = bitpos;         /* index whose binary indices form an edge */
            idx_t t_i = i;
            idx_t vert = 1;           /* vertex labels are 1‑based within the edge */

            while (t_i) {
                if (t_i & 1ULL) {
                    deg[vert] += (seq_t)1;
                    present[vert] = true;
                }
                t_i >>= 1;
                ++vert;
            }
        }
        tmp >>= 1;
        ++bitpos;
    }

    /* Find the minimum degree among vertices that appear. */
    seq_t min_deg = 0;
    bool first = true;
    for (idx_t v = 1; v < 65; ++v) {
        if (present[v]) {
            if (first) {
                min_deg = deg[v];
                first = false;
            } else if (deg[v] < min_deg) {
                min_deg = deg[v];
            }
        }
    }
    return first ? 0 : min_deg;   /* first should be false for n > 0 */
}
