/*
 * A245868  Number of length n+2 0..7 arrays with some pair in every
 *          consecutive three terms totalling exactly 7.
 * https://oeis.org/A245868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=168, a(2)=712, a(3)=2368,
 * a(n)=2*a(n-1)+6*a(n-2)-a(n-3) for n>=4 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)  return (seq_t)168;
    if (n == 2)  return (seq_t)712;
    if (n == 3)  return (seq_t)2368;

    seq_t a_nm3 = (seq_t)168;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)712;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)2368; /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* next = 2*a_nm1 + 6*a_nm2 - a_nm3 */
        next = a_nm1 + a_nm1;          /* 2*a_nm1 */
        seq_t t = a_nm2;
        t = t + t + t + t + t + t;     /* 6*a_nm2 */
        next = next + t;
        next = next - a_nm3;           /* subtract a_nm3 */

        /* slide the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;
}
