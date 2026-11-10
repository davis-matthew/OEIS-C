/*
 * A283325  Lengths of runs of successive zeros in A283683.
 * https://oeis.org/A283325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th maximal run of zeros in A283683, found
 *            by scanning A283683 (a(1)=0, a(2)=1, T(i,j) read by
 *            antidiagonals with every row equal to the sequence)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9177 (2026-11-08)
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
    idx_t count = 0;
    idx_t run = 0;
    idx_t m = 1;
    for (;;) {
        if (A283683_bit(m) == 0) {
            run++;
        } else {
            if (run > 0) {
                count++;
                if (count == n) return (seq_t)run;
                run = 0;
            }
        }
        m++;
    }
}
