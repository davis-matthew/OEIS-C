/*
 * A276646  a(n) = floor(Sum_{k=1..n} 0.k).
 * https://oeis.org/A276646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{k=1..n} k / 10^{len(k)} )
 * Range:     u8 n <= 9, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int digits_uint64(idx_t x)
{
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    int max_digits = digits_uint64(n);
    seq_t pow10[21];
    pow10[0] = (seq_t)1;
    for (int i = 1; i <= max_digits; ++i)
        pow10[i] = pow10[i-1] * (seq_t)10;

    seq_t denom = pow10[max_digits];
    seq_t numer = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        int d = digits_uint64(k);
        numer += (seq_t)k * pow10[max_digits - d];
    }

    return numer / denom;
}
