/*
 * A253439  Number of (5+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A253439: Number of (5+1) × (n+1) 0..1 arrays with every 2×2
   subblock diagonal minus antidiagonal sum nondecreasing horizontally,
   vertically and along the NE‑to‑SW antidiagonals.

   Formula used:
     For n ≤ 3      : a(n) = [181, 180, 193]
     For 4 ≤ n ≤ 5  : a(n) = [228, 300]
     For n ≥ 6      : a(n+1) = 3·a(n) – 2·a(n−1)   (recurrence)
     Equivalently, for n > 3: a(n) = 9·2^(n−1) + 156.
   The recurrence is efficient and works for all n with arbitrary
   precision; the explicit formula is used only for the first few terms.
*/

#include <stdbool.h>

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 1) return (seq_t)181;
    if (n == 2) return (seq_t)180;
    if (n == 3) return (seq_t)193;
    if (n == 4) return (seq_t)228;
    if (n == 5) return (seq_t)300;

    /* For n ≥ 6 compute using the linear recurrence:
          a(n) = 3*a(n-1) - 2*a(n-2)  */
    seq_t prev2 = (seq_t)228;   /* a(4) */
    seq_t prev1 = (seq_t)300;   /* a(5) */
    seq_t curr  = prev1;        /* placeholder */

    /* Loop from 6 up to n inclusive */
    for (idx_t i = 6; i <= n; ++i) {
        curr = 3 * prev1 - 2 * prev2;  /* recurrence */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
