/*
 * A359124  Concatenate the decimal numbers 1,2,3,...,n, then add 1.
 * https://oeis.org/A359124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(1,2,3,...,n)+1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t pow10_idx(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i)
        p *= 10;
    return p;
}

static int digit_count_idx(idx_t x)
{
    int cnt = 1;
    while (x >= 10) {
        x /= 10;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1, but guard against 0 */

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int d = digit_count_idx(i);
        seq_t mul = pow10_idx(d);
        res = res * mul + (seq_t)i;
    }
    res += 1;
    return res;
}
