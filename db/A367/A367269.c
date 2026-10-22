/*
 * A367269  Triangle T(n, k) read by rows and based on A042948 yields a
 *          permutation of the natural numbers.
 * https://oeis.org/A367269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) is the row
 *            number, k = n - r(r+1)/2 is the column number, and T(n,k) =
 *            (n+1)(n+2)/2 + n*(n mod 2) - 2*k + (k mod 2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t compute_row(idx_t n)
{
    idx_t low = 0, high = n, r = 0;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = compute_row(n);
    __int128 tri_r = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri_r;

    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    seq_t base = ((rn + 1) * (rn + 2)) / 2 + rn * (rn % 2);
    return base - 2 * kn + (kn % 2);
}
