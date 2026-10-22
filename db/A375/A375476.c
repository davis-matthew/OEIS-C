/*
 * A375476  a(3*n)=A001045(n+1), a(3*n+1)=A084214(n), a(3*n+2)=A000079(n) for
 *          n >= 0.
 * https://oeis.org/A375476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + 2*a(n-6) for n >= 8, with initial values
   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=3, a(7)=4. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n <= 7)
    {
        static const seq_t base[8] = {1, 1, 1, 1, 1, 2, 3, 4};
        return base[n];
    }

    /* Allocate temporary storage for all computed terms up to n */
    seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!buf)
        return (seq_t)0;   /* allocation failure – unlikely in this context */

    /* Initialize the known terms */
    buf[0] = 1; buf[1] = 1; buf[2] = 1;
    buf[3] = 1; buf[4] = 1; buf[5] = 2;
    buf[6] = 3; buf[7] = 4;

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = 8; i <= n; ++i)
    {
        buf[i] = buf[i - 3] + ((seq_t)2) * buf[i - 6];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
