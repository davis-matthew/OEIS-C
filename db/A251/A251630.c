/*
 * A251630  Column sums of the n X n square array filled with numbers from 1
 *          to n^2, row by row, from left to right.
 * https://oeis.org/A251630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = n * floor((1 + sqrt(1 + 8*(n-1))) / 2)
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the sequence A251630 – column sum counts. */

#include <stdint.h>

static unsigned __int128 int128_sqrt(unsigned __int128 v)
{
    unsigned __int128 low = 0, high = v, ans = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            low = mid + 1;
            ans = mid;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 for safety */

    /* compute 1 + 8*(n-1) safely in 128‑bit */
    unsigned __int128 val = 8 * ((unsigned __int128)n - 1) + 1;

    /* floor(sqrt(val)) */
    unsigned __int128 sq = int128_sqrt(val);

    /* factor = floor((1 + sqrt) / 2) */
    idx_t factor = (idx_t)((sq + 1) >> 1);

    return (seq_t)n * (seq_t)factor;
}
