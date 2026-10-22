/*
 * A398247  Number of feedback vertex sets in the grid graph P_n X P_4.
 * https://oeis.org/A398247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 13*a(n-1) + 19*a(n-2) - 90*a(n-3) + 2*a(n-4)
          + 94*a(n-5) - 72*a(n-6) - 8*a(n-7) - 192*a(n-8)
   Initial values:
   a(0)=1, a(1)=16, a(2)=216, a(3)=3021, a(4)=41953,
   a(5)=583464, a(6)=8112115, a(7)=112792703
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[8] = {
        (seq_t)1,
        (seq_t)16,
        (seq_t)216,
        (seq_t)3021,
        (seq_t)41953,
        (seq_t)583464,
        (seq_t)8112115,
        (seq_t)112792703
    };

    /* Return directly for the first eight values. */
    if ((size_t)n <= 7)
        return base[(size_t)n];

    /* Sliding buffer holding the last eight values. */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    /* Compute the sequence up to n. */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t a1 = buf[(size_t)((i - 1) & 7)];
        seq_t a2 = buf[(size_t)((i - 2) & 7)];
        seq_t a3 = buf[(size_t)((i - 3) & 7)];
        seq_t a4 = buf[(size_t)((i - 4) & 7)];
        seq_t a5 = buf[(size_t)((i - 5) & 7)];
        seq_t a6 = buf[(size_t)((i - 6) & 7)];
        seq_t a7 = buf[(size_t)((i - 7) & 7)];
        seq_t a8 = buf[(size_t)((i - 8) & 7)];

        seq_t pos = (seq_t)13 * a1 + (seq_t)19 * a2 + (seq_t)2 * a4 + (seq_t)94 * a5;
        seq_t neg = (seq_t)90 * a3 + (seq_t)72 * a6 + (seq_t)8 * a7 + (seq_t)192 * a8;
        buf[(size_t)(i & 7)] = pos - neg;
    }

    return buf[(size_t)(n & 7)];
}
