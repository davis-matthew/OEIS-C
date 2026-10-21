/*
 * A277952  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 14", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A277952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-2) – 100*a(n-3), with initial values
 *            a(0)=1, a(1)=11, a(2)=110
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *buf = NULL; /* cached sequence values */
    static size_t cap  = 0;   /* allocated capacity */
    static size_t len  = 0;   /* number of valid entries in buf, i.e. len-1 is max index stored */

    /* Ensure the first three terms are present. */
    if (len == 0) {
        /* Allocate space for at least 3 entries. */
        grow_seq(&buf, &cap, (size_t)3);
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)11;
        buf[2] = (seq_t)110;
        len = 3;
    }

    /* Compute any missing terms up to n. */
    if (n >= len) {
        size_t need = (size_t)(n + 1);
        grow_seq(&buf, &cap, need);

        /* Incrementally fill the new entries. */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < 3) {
                /* already set when len == 0 */
                continue;
            }
            buf[i] = buf[i - 1] + (seq_t)100 * buf[i - 2] - (seq_t)100 * buf[i - 3];
        }
        len = need;
    }

    return buf[(size_t)n];
}
