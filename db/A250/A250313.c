/*
 * A250313  Number of length n+2 0..1 arrays with the sum of second
 *          differences squared multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A250313):
   a(n) = 3*a(n-1) - 6*a(n-3) + 3*a(n-4) + 3*a(n-5) - 2*a(n-6)  (n ≥ 7)
   Initial values:
   a(1)=2, a(2)=8, a(3)=8, a(4)=24, a(5)=42, a(6)=104
*/
#include <stdint.h>

seq_t a(idx_t n) {
    /* handle the offset 1 */
    if (n < 1) return 0; /* undefined for n < 1 */

    /* small n using the tabulated values */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)24;
    if (n == 5) return (seq_t)42;
    if (n == 6) return (seq_t)104;

    /* Iteratively compute by the recurrence, keeping the six needed
       previous terms only. */
    seq_t a1 = (seq_t)2;   /* a(n-6)  */
    seq_t a2 = (seq_t)8;   /* a(n-5)  */
    seq_t a3 = (seq_t)8;   /* a(n-4)  */
    seq_t a4 = (seq_t)24;  /* a(n-3)  */
    seq_t a5 = (seq_t)42;  /* a(n-2)  */
    seq_t a6 = (seq_t)104; /* a(n-1)  */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) from the recurrence.  All intermediate
           quantities are non‑negative because the true value is
           positive. */
        seq_t part1 = 3 * a6;   /* 3*a(n-1)  */
        seq_t part2 = 3 * a3;   /* 3*a(n-4)  */
        seq_t part3 = 3 * a2;   /* 3*a(n-5)  */
        seq_t sub1  = 6 * a4;   /* 6*a(n-3)  */
        seq_t sub2  = 2 * a1;   /* 2*a(n-6)  */

        seq_t sum_pos = part1 + part2 + part3;        /* all plus parts   */
        seq_t tmp     = sum_pos - sub1;               /* - 6*a(n-3)       */
        seq_t new_val = tmp - sub2;                   /* - 2*a(n-6)       */

        /* shift the window forward for the next iteration */
        a1 = a2; a2 = a3; a3 = a4;
        a4 = a5; a5 = a6; a6 = new_val;
    }
    return a6;
}
