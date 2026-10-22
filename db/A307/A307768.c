/*
 * A307768  Number of n-step random walks on a line starting from the origin
 *          and returning to it at least once.
 * https://oeis.org/A307768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(0)=0, a(1)=0, a(2)=2, a(3)=4; for n≥4, a(n) = 2*a(n-1) +
 *            (4*(n-3)/n) * (a(n-2)-2*a(n-3))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 4;

    seq_t f_nm3 = 0;   /* a(n-3) */
    seq_t f_nm2 = 0;   /* a(n-2) */
    seq_t f_nm1 = 2;   /* a(n-1) - at start n=3 gives a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t curr = mul_small(f_nm1, 2);  /* 2*a(n-1) */
        if (i >= 4)
        {
            seq_t diff = f_nm2 - mul_small(f_nm3, 2);   /* a(n-2) - 2*a(n-3) */
            idx_t factor = 4 * (i - 3);                /* 4*(n-3) */
            seq_t add   = mul_small(diff, (int)factor) / i; /* (4*(n-3)*diff)/n */
            curr += add;
        }
        f_nm3 = f_nm2;
        f_nm2 = f_nm1;
        f_nm1 = curr;
    }
    return f_nm1;
}
