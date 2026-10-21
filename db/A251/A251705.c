/*
 * A251705  4-step Fibonacci sequence starting with 1, 1, 1, 0.
 * https://oeis.org/A251705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n) + a(n+1) + a(n+2) + a(n+3); a(0) = 1, a(1) = 1,
 *            a(2) = 1, a(3) = 0
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t and seq_t definitions */

/* Compute the 4‑step Fibonacci sequence A251705 */
seq_t a(idx_t n)
{
    /* Handle the first four seed values directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 0;

    /* Sliding window of the last four terms */
    seq_t t0 = 1;   /* a(n-4) */
    seq_t t1 = 1;   /* a(n-3) */
    seq_t t2 = 1;   /* a(n-2) */
    seq_t t3 = 0;   /* a(n-1) */
    seq_t tn;       /* current term a(n) */

    /* Iteratively compute up to a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        tn = t0 + t1 + t2 + t3;  /* use the recurrence */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = tn;
    }

    return tn;
}
