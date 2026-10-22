/*
 * A353582  a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4) - 1, with a(0) = 0 = a(1),
 *          a(2) = 2, and a(3) = 3.
 * https://oeis.org/A353582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4) - 1
   Initial terms: a(0)=0, a(1)=0, a(2)=2, a(3)=3 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0u) return 0;
    if (n == 1u) return 0;
    if (n == 2u) return 2;
    if (n == 3u) return 3;

    /* compute iteratively, keeping only the last five values */
    seq_t prev4 = 0;   /* a(0) */
    seq_t prev3 = 0;   /* a(1) */
    seq_t prev2 = 2;   /* a(2) */
    seq_t prev1 = 3;   /* a(3) */
    seq_t curr;

    for (idx_t i = 4u; i <= n; ++i) {
        curr = 2 * prev1 + 2 * prev3 + prev4 - 1; /* a(i) */
        /* shift the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1; /* now prev1 == a(n) */
}
