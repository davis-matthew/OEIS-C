/*
 * A242067  Number of triangular numbers between n^2 and n^3 (excluding the
 *          bounds).
 * https://oeis.org/A242067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of triangular numbers between n^2 and n^3
 *            (excluding bounds)
 * Range:     u8 n <= 36, u16 n <= 1314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Compute k_min = floor((sqrt(1+8n^2)-1)/2) + 1 */
    unsigned __int128 valA = (unsigned __int128)8 * n * n + 1;
    idx_t sqrtA = floor_sqrt((idx_t)valA);
    idx_t k_min = (sqrtA - 1) / 2 + 1;

    /* Compute k_max = floor((sqrt(1+8n^3)-1)/2) with adjustment for exact square */
    unsigned __int128 valB = (unsigned __int128)8 * n * n * n + 1;
    idx_t sqrtB = floor_sqrt((idx_t)valB);
    idx_t k_max = (sqrtB - 1) / 2;
    if ((unsigned __int128)sqrtB * sqrtB == valB)
        k_max -= 1;

    if (k_max < k_min)
        return 0;

    return (seq_t)(k_max - k_min + 1);
}
