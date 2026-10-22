/*
 * A335262  Triangle of triangular numbers, read by rows, constructed like
 *          this: Given a sequence t, start row 0 with t(0). Compute row n for
 *          n > 0 by reversing row n-1 and prepending t(n). The sequence t is
 *          here chosen as the triangular numbers.
 * https://oeis.org/A335262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((2*k-1-r)*(2*k-r))/2, where r = floor((sqrt(8*n+1)-1)/2)
 *            (row index), k = n - r*(r+1)/2 (index within the row)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __uint128_t row_start(uint64_t r)
{
    /* start index of row r = r*(r+1)/2 */
    return (__uint128_t)r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* first term of the triangle */

    /* Find the smallest r such that (r+1)(r+2)/2 > n
       which is the row containing the nth element. */
    uint64_t low = 0, high = 1;
    while (((__uint128_t)(high + 1) * (high + 2)) / 2 <= n)
        high <<= 1;
    low = high >> 1;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (((__uint128_t)(mid + 1) * (mid + 2)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    uint64_t r = high;                    /* row index */

    uint64_t start = (uint64_t)row_start(r);   /* first index of this row */
    uint64_t k = n - start;                /* in‑row position (0‑based) */

    long long a1 = 2LL * (long long)k - 1LL - (long long)r;
    long long b1 = 2LL * (long long)k - (long long)r;

    __int128 prod = (__int128)a1 * (__int128)b1;
    return (seq_t)(prod / 2);
}
