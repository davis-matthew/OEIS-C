/*
 * A319135  Irregular triangle read by rows in which row n lists the positive
 *          divisors of n that are <= sqrt(n) in decreasing order.
 * https://oeis.org/A319135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A319135: Irregular triangle read by rows.
    Each row n lists the positive divisors of n that are <= sqrt(n)
    in decreasing order.  a(k) is the k‑th element of the flattened
    triangle.  This implementation computes a(k) by iterating
    through the rows and counting the divisors until the desired
    position is reached.  It uses only integer arithmetic and
    a small 64‑bit integer square‑root routine. */
#include <stdint.h>

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t res = 0;
    for (int i = 32; i >= 0; --i) {
        idx_t bit = (idx_t)1 << i;
        idx_t trial = res | bit;
        if ((unsigned __int128)trial * trial <= n)
            res = trial;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms seen so far */
    for (idx_t row = 1; ; ++row) {
        idx_t limit = floor_sqrt_u64(row);
        for (idx_t d = limit; ; --d) {
            if (row % d == 0) {
                ++count;
                if (count == n)
                    return (seq_t)d;
            }
            if (d == 1)
                break;      /* after processing d==1 exit the loop */
        }
    }
}
