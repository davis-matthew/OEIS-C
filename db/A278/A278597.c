/*
 * A278597  One half of A278481.
 * https://oeis.org/A278597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(1)=1; for n > 1, let i be the row number (the unique integer
 *            with i*(i+1)/2 ≥ n) and j = n – (i-1)*i/2 be the position in
 *            that row. Then a(n) = 2 if j = 1 or j = i (border), otherwise
 *            a(n) = 3
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   // out of bounds, not used by the caller
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;          /* special first term */

    /* binary search to find the smallest i with i*(i+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t i = low;                  /* row number */

    /* column index j = n - T_{i-1} */
    __int128 tri_prev = (__int128)(i - 1) * i / 2;
    idx_t j = n - (idx_t)tri_prev;

    if (j == 1 || j == i)
        return (seq_t)2;
    return (seq_t)3;
}
