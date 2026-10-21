/*
 * A268623  Number of n X 3 0..2 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) initial
 *            values: a(1) = 24 a(2) = 174 a(3) = 1086 a(4) = 6300
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The following implementation is completely stateless and recomputes
   the sequence up to the requested index.  No dynamic memory allocation
   is performed, so it works both as C11 and as C++17, regardless of
   the underlying integer type used for seq_t.  */
seq_t a(idx_t n)
{
    /* Return base values directly for n <= 4 */
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)174;
    if (n == 3) return (seq_t)1086;
    if (n == 4) return (seq_t)6300;

    /* variables hold the last four sequence values:
         a1 = a(k-3), a2 = a(k-2), a3 = a(k-1), a4 = a(k)   */
    seq_t a1 = (seq_t)24;
    seq_t a2 = (seq_t)174;
    seq_t a3 = (seq_t)1086;
    seq_t a4 = (seq_t)6300;

    /* Compute a(5) .. a(n) iteratively.  The loop runs (n-4) times. */
    for (idx_t i = 5; i <= n; ++i)
    {
        /* recurrence: a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) */
        seq_t next = (seq_t)10 * a4
                   - (seq_t)29 * a3
                   + (seq_t)20 * a2
                   - (seq_t) 4 * a1;

        /* shift the window: a(n-3) -> a1, a(n-2) -> a2, a(n-1) -> a3, a(n) -> a4 */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
