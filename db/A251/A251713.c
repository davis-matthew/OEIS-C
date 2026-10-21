/*
 * A251713  7-step Fibonacci sequence starting with (0,0,1,0,0,0,0).
 * https://oeis.org/A251713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 7‑step Fibonacci sequence
 * Recurrence: a(n+7) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)
 * Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=0, a(5)=0, a(6)=0
 */
#include <stdbool.h>   /* for completeness, may not be needed */

seq_t a(idx_t n)
{
    /* static initial values for n < 7 */
    static const seq_t init[7] = { (seq_t)0, (seq_t)0, (seq_t)1,
                                   (seq_t)0, (seq_t)0, (seq_t)0,
                                   (seq_t)0 };

    if (n < 7)
        return init[n];

    /* sliding window of the last 7 terms */
    seq_t win[7];
    for (idx_t i = 0; i < 7; ++i)
        win[i] = init[i];

    /* sum of the current window */
    seq_t sum = 0;
    for (idx_t j = 0; j < 7; ++j)
        sum += win[j];

    /* compute terms up to n */
    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t next = sum;          /* next term = sum of previous 7 */
        sum += next;               /* add new term to sum */
        sum -= win[i % 7];         /* remove term that is leaving the window */
        win[i % 7] = next;         /* insert new term */
    }

    return win[n % 7];
}
