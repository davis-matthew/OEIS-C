/*
 * A252924  Number of n X 2 nonnegative integer arrays with upper left 0 and
 *          lower right n+2-6 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..5} c[k] * binom(n + 4 - k, 8) with c =
 *            {1,10,-15,3,3,-1} (series from the GF) The sequence starts with
 *            a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 56, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute binomial coefficient (x choose 8) using 128‑bit arithmetic.
   x is guaranteed to be ≤10 000 in the test harness, so the result
   fits in 128 bits. */
static inline __int128 binom8(idx_t x)
{
    __int128 prod = 1;
    for (idx_t i = 0; i < 8; ++i)
        prod *= ( (__int128)x - i );
    return prod / 40320;          /* 8! = 40320 */
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    static const long long coeff[6] = { 1, 10, -15, 3, 3, -1 };
    __int128 result = 0;

    for (int k = 0; k < 6; ++k)
    {
        idx_t shift = n + 4 - k;
        if (shift < 8)          /* binom(x,8) is zero for x<8 */
            continue;
        result += (__int128)coeff[k] * binom8(shift);
    }

    return (seq_t)result;
}
