/*
 * A339050  Triangle read by rows T(n, m) = F(2*m-1)*(n-m) + F(2*m), for 1 <=
 *          m <= n, where F = A000045 (Fibonacci).
 * https://oeis.org/A339050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = F(2*m-1)*(r-m) + F(2*m) * where r = smallest integer with
 *            r*(r+1)/2 ≥ n * m = n - r*(r-1)/2 * and F(k) is the k-th
 *            Fibonacci number (F(1)=1, F(2)=1)
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

static idx_t
row_from_index(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    /* n ≥ 1 */
    idx_t r = row_from_index(n);

    /* compute r*(r-1)/2 safely */
    unsigned __int128 prefix = (unsigned __int128)(r - 1) * r / 2;
    idx_t offset = n - (idx_t)prefix - 1;   /* 0‑based position in row r */
    idx_t m = offset + 1;                   /* column number */

    seq_t f2m_minus1 = fib(2 * m - 1);
    seq_t f2m       = fib(2 * m);

    return f2m_minus1 * (seq_t)(r - m) + f2m;
}
