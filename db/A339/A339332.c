/*
 * A339332  Sums of antidiagonals in A283683.
 * https://oeis.org/A339332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A283683(k), where A283683(1)=0, A283683(2)=1
 *            and A283683(m) = A283683(d-p+1) with d = ceil((sqrt(8m+1)-1)/2),
 *            p = m - d(d-1)/2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A283683_bit(idx_t n)
{
    if (n <= 1) return 0;
    if (n == 2) return 1;
    while (n > 2) {
        idx_t low = 1, high = 1;
        while ((unsigned __int128)high * (high + 1) / 2 < n)
            high <<= 1;
        while (low < high) {
            idx_t mid = (low + high) >> 1;
            if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
                high = mid;
            else
                low = mid + 1;
        }
        idx_t d = high;
        idx_t tprev = d * (d - 1) / 2;
        idx_t p = n - tprev;
        n = d - p + 1;
    }
    return (n == 1) ? 0 : 1;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t k = 1; k <= n; k++) {
        if (A283683_bit(k))
            s += 1;
    }
    return s;
}
