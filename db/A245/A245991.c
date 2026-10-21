/*
 * A245991  Number of length n+2 0..4 arrays with no pair in any consecutive
 *          three terms totalling exactly 4.
 * https://oeis.org/A245991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A245991: Number of length n+2 arrays with values 0..4
   that contain no consecutive triple summing to exactly 4.
   Recurrence (order 4):
     a(1) = 64
     a(2) = 208
     a(3) = 676
     a(4) = 2196
     a(n) = 2*a(n-1) + a(n-2) + 9*a(n-3) + 3*a(n-4)   (n >= 5)
*/

seq_t a(idx_t n)
{
    /* Return 0 for indices outside the defined range (n < 1). */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Handle the first four values directly. */
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)208;
    if (n == 3) return (seq_t)676;
    if (n == 4) return (seq_t)2196;

    /* Variables that hold the last four terms of the sequence. */
    seq_t a1 = (seq_t)64;   /* a(n-4) */
    seq_t a2 = (seq_t)208;  /* a(n-3) */
    seq_t a3 = (seq_t)676;  /* a(n-2) */
    seq_t a4 = (seq_t)2196; /* a(n-1) */
    seq_t an = (seq_t)0;

    /* Iterate from 5 up to n, updating the terms as per the recurrence. */
    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)2 * a4 + a3 + (seq_t)9 * a2 + (seq_t)3 * a1;
        /* Shift the sliding window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;
}
