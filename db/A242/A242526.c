/*
 * A242526  Number of cyclic arrangements of S={1,2,...,n} such that the
 *          difference between any two neighbors is at most 4.
 * https://oeis.org/A242526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A242526
   Number of cyclic arrangements of S={1,2,…,n} such that
   the difference between any two neighbors is at most 4.
   Recurrence (valid for n>20):

   a(n) = 2*a(n-1) + a(n-2) - a(n-4) + 9*a(n-5) + 5*a(n-6)
          - a(n-7) - 7*a(n-8) -10*a(n-9) + 2*a(n-10)
          + 2*a(n-11)+2*a(n-12)+4*a(n-13)-2*a(n-17)-a(n-18)

   Initial values (n = 1 … 20):
   1, 1, 1, 3, 12, 36, 90, 214, 521, 1335,
   3473, 9016, 23220, 59428, 152052, 389636,
   999776, 2566517, 6586825, 16899574
*/
#include <stddef.h>  /* for size_t */

static const seq_t init_vals[20] = {
    (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)3,   (seq_t)12,
    (seq_t)36, (seq_t)90, (seq_t)214, (seq_t)521, (seq_t)1335,
    (seq_t)3473,(seq_t)9016,(seq_t)23220,(seq_t)59428,(seq_t)152052,
    (seq_t)389636,(seq_t)999776,(seq_t)2566517,(seq_t)6586825,(seq_t)16899574
};

seq_t a(idx_t n)
{
    /* Offset safety: a(0) is undefined; return 0 for n==0. */
    if (n == 0) return (seq_t)0;

    /* For n <= 20, return the known initial value. */
    if (n <= 20) return init_vals[n - 1];

    /* Sliding window of the last 20 values needed for the recurrence. */
    seq_t prev[20];
    /* Initialise with a(20) .. a(1) in reverse order: */
    for (size_t k = 0; k < 20; ++k)
        prev[k] = init_vals[19 - k];

    seq_t next = (seq_t)0;
    /* Compute from a(21) up to a(n). */
    for (idx_t i = 21; i <= n; ++i)
    {
        /* Recurrence using the sliding window:
           prev[0] = a(i-1), prev[1] = a(i-2), …   */
        next = (seq_t)2 * prev[0]           + prev[1]
               - prev[3]                     + (seq_t)9 * prev[4]
               + (seq_t)5 * prev[5]          - prev[6]
               - (seq_t)7 * prev[7]          - (seq_t)10 * prev[8]
               + (seq_t)2 * prev[9]          + (seq_t)2 * prev[10]
               + (seq_t)2 * prev[11]         + (seq_t)4 * prev[12]
               - (seq_t)2 * prev[16]         - prev[17];

        /* Shift the window to make room for the new value. */
        for (size_t k = 19; k > 0; --k)
            prev[k] = prev[k - 1];
        prev[0] = next;
    }
    return next;
}
