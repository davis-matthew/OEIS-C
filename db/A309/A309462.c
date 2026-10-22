/*
 * A309462  Limiting row sequence for Pascal-like triangle A140995 (Stepan's
 *          triangle with index of asymmetry s = 3).
 * https://oeis.org/A309462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) + a(n-4) for n >= 4 with
 *            initial terms a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial four values */
    const seq_t init[4] = {1, 2, 4, 8};

    if (n < 4)
        return init[(size_t)n];

    seq_t a0 = init[0]; /* a(k-4) */
    seq_t a1 = init[1]; /* a(k-3) */
    seq_t a2 = init[2]; /* a(k-2) */
    seq_t a3 = init[3]; /* a(k-1) */
    seq_t res = a3;     /* placeholder for a(k) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(k) = a(k-1) + a(k-2) + 2*a(k-3) + a(k-4) */
        res = a3 + a2 + a1 + a1 + a0; /* 2*a1 is a1+a1 */

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }

    return res;
}
