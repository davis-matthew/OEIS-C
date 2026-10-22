/*
 * A352579  Irregular triangle T(n,k) (n >= 1, 0 <= k <= 2^n-2) read by rows:
 *          row n = 1 is [0]; thereafter, T(n,k) = 2*T(n-1,k)+(k mod 2) and
 *          T(n,k+2^(n-1)-1) = 2*T(n-1,k)+(k+1 mod 2) for k = 0..2^(n-1)-2;
 *          T(n,2^n-2) = 2^n-2.
 * https://oeis.org/A352579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element (starting at 1) of the irregular
 *            triangle T(n,k) read row by row. The triangle is defined by
 *            T(1,0)=0 and for n≥2, let s=2^(n-1)-1. For 0≤k<s: T(n,k) =
 *            2*T(n-1,k) + (k mod 2) For 0≤k<s: T(n,k+s) = 2*T(n-1,k) + ((k+1)
 *            mod 2) T(n,2^n-2) = 2^n-2
 * Range:     u8 n <= 504, u16 n <= 9962, u32 n <= 9962, u64 n <= 9962,
 *            u128 n <= 9962, big any n
 * Helpers:   _T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_T.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n<1, return 0 as a guard */

    /* find the row that contains position n */
    idx_t cumulative = 0;
    idx_t r = 0;
    for (;;)
    {
        ++r;
        idx_t len = ((idx_t)1 << r) - 1;     /* length of row r */
        cumulative += len;
        if (n <= cumulative)                /* n lies in this row */
        {
            idx_t prev_cumulative = cumulative - len;
            idx_t k = n - prev_cumulative - 1;   /* 0‑based column in row r */
            return _T(r, k);
        }
    }
}
