/*
 * A350114  Number of Deutsch paths with peaks at odd height.
 * https://oeis.org/A350114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350114
   Closed Deutsch paths of n steps with all peaks at odd height.
   Recurrence (valid for n ≥ 5):

        (n+1)·a(n) = (n-2)·a(n-1) + 3(n-1)·a(n-2)
                    + 3(n-3)·a(n-3) + (n-2)·a(n-4) – (n-5)·a(n-5)

   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=2.
   The function below evaluates a(n) for any n≥0 using only this
   recurrence.  It runs in O(n) time and uses only stack storage,
   so it works for all build configurations (integer or arbitrary‑precision).
*/

#include <stddef.h>

/* Compute a(n).  The function is independent of any external
   allocation utilities – it simply iterates the recurrence. */
seq_t a(idx_t n)
{
    /* Handle known base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)2;

    /* Cache of the last five computed values.
       values[0] = a(n-5), values[1] = a(n-4), …,
       values[4] = a(n-1).  The array is updated at each step. */
    seq_t values[5] = { (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)2 };

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute the numerator of the recurrence. */
        seq_t term1 = (seq_t)(i - 2) * values[4];           /* (i-2)*a(i-1) */
        seq_t term2 = (seq_t)3 * (i - 1) * values[3];        /* 3(i-1)*a(i-2) */
        seq_t term3 = (seq_t)3 * (i - 3) * values[2];        /* 3(i-3)*a(i-3) */
        seq_t term4 = (seq_t)(i - 2) * values[1];           /* (i-2)*a(i-4) */
        seq_t term5 = (seq_t)(i - 5) * values[0];           /* (i-5)*a(i-5) */

        seq_t num = term1 + term2 + term3 + term4 - term5;
        seq_t denom = (seq_t)(i + 1);
        seq_t ai = num / denom;

        /* Shift the circular buffer for the next iteration. */
        values[0] = values[1];
        values[1] = values[2];
        values[2] = values[3];
        values[3] = values[4];
        values[4] = ai;
    }

    /* After the loop, values[4] holds a(n). */
    return values[4];
}
