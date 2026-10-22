/*
 * A351319  a(n) = floor(n/k), where k is the nearest square to n.
 * https://oeis.org/A351319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/k), where k = nearest square to n, i.e., k =
 *            round(sqrt(n))^2
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return floor(sqrt(n)) for 0 ≤ n ≤ UINT64_MAX.
   Implemented by binary search; result fits in 0 … 0xFFFFFFFF. */
static idx_t floor_isqrt_u64(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t low = 1, high = 0xFFFFFFFFULL, mid;
    while (low <= high)
    {
        mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(n)) */
    idx_t s = floor_isqrt_u64(n);

    /* Squares of s and s+1 */
    idx_t s2 = s * s;
    idx_t next = s + 1;

    /* If s is the largest possible (2^32‑1) then next overflows;
       in that case the nearest square is s2. */
    if (next == 0)
        return (seq_t)(n / s2);

    idx_t next2 = next * next;

    /* Choose the nearest square using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 diff_s   = (unsigned __int128)n - s2;
    unsigned __int128 diff_next = (unsigned __int128)next2 - n;
    idx_t k = (diff_s <= diff_next) ? s2 : next2;

    /* Result is floor(n / k) */
    idx_t div = n / k;
    return (seq_t)div;
}
