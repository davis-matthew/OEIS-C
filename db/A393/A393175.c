/*
 * A393175  Sum of the heights of symmetric peaks over all compositions of the
 *          integer n.
 * https://oeis.org/A393175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=a(3)=0, a(4)=1;
   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) - 4*a(n-4) + 4*a(n-5)  for n ≥ 5 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0,1,2,3,4 */
    static const seq_t init[5] = {0, 0, 0, 0, 1};

    if (n < 5)
        return init[n];

    /* Keep the last five values a(n-5), … , a(n-1) */
    seq_t a0 = init[0];   /* a(n-5) */
    seq_t a1 = init[1];   /* a(n-4) */
    seq_t a2 = init[2];   /* a(n-3) */
    seq_t a3 = init[3];   /* a(n-2) */
    seq_t a4 = init[4];   /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = a4 * 4;          /* 4*a(n-1) */
        next -= a3 * 4;              /* -4*a(n-2) */
        next += a2;                  /* +a(n-3)  */
        next -= a1 * 4;              /* -4*a(n-4) */
        next += a0 * 4;              /* +4*a(n-5) */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;   /* a(n) */
}
