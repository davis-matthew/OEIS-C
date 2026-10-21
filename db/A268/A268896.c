/*
 * A268896  Start at a(0)=1. a(n) = a(n-1)+2 if n == 1,2 (mod 3) and
 *          a(n)=a(n-1)+a(n-3) if n == 0 (mod 3).
 * https://oeis.org/A268896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   if n%3==1 or n%3==2 then a(n)=a(n-1)+2,
   if n%3==0   then a(n)=a(n-1)+a(n-3). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* a(0) = 1 */
    seq_t a0 = (seq_t)1;
    /* a(1) = a(0)+2 */
    if (n == 1) {
        return a0 + (seq_t)2;
    }
    seq_t a1 = a0 + (seq_t)2;
    /* a(2) = a(1)+2 */
    if (n == 2) {
        return a1 + (seq_t)2;
    }
    seq_t a2 = a1 + (seq_t)2;

    /* For i ≥ 3 keep three consecutive terms:
       prev3 = a(i-3), prev2 = a(i-2), prev1 = a(i-1) */
    seq_t prev3 = a0;
    seq_t prev2 = a1;
    seq_t prev1 = a2;
    seq_t curr = prev1; /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        if (i % 3 == 0) {
            curr = prev1 + prev3;          /* a(i) = a(i-1)+a(i-3) */
        } else {
            curr = prev1 + (seq_t)2;       /* a(i) = a(i-1)+2 */
        }
        /* shift window for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
