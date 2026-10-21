/*
 * A241965  Number of length 2+3 0..n arrays with no consecutive four elements
 *          summing to more than 2*n.
 * https://oeis.org/A241965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (23*n^5 + 135*n^4 + 315*n^3 + 375*n^2 + 232*n + 60)/60
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 101, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* not part of the OEIS offset, but harmless */
        return 0;

    /* For small builtin integer types use 128‑bit intermediates so that
       all partial products fit.  For larger (arbitrary‑precision) types
       the arithmetic is performed directly on seq_t. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn = (unsigned __int128)n;
        unsigned __int128 n2 = nn * nn;
        unsigned __int128 n3 = n2 * nn;
        unsigned __int128 n4 = n3 * nn;
        unsigned __int128 n5 = n4 * nn;

        unsigned __int128 num =
            (unsigned __int128)23 * n5 +
            (unsigned __int128)135 * n4 +
            (unsigned __int128)315 * n3 +
            (unsigned __int128)375 * n2 +
            (unsigned __int128)232 * nn +
            (unsigned __int128)60;

        return (seq_t)(num / 60);
    } else {
        seq_t nn  = (seq_t)n;
        seq_t n2  = nn * nn;
        seq_t n3  = n2 * nn;
        seq_t n4  = n3 * nn;
        seq_t n5  = n4 * nn;

        seq_t num = 23 * n5 + 135 * n4 + 315 * n3 + 375 * n2 + 232 * nn + 60;
        return num / 60;
    }
}
