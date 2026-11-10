/*
 * A345201  Bit-reverse the odd part of the Zeckendorf representation of n:
 *          a(n) = A022290(A057889(A003714(n))).
 * https://oeis.org/A345201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A022290(A057889(A003714(n))): take the Zeckendorf
 *            (fibbinary) word of n, bit-reverse its odd part keeping trailing
 *            zeros, then read the result with 2^k replaced by Fibonacci(k+2)
 * Range:     u8 n <= 54, u16 n <= 2645, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 84 terms through n = 9149 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A345201_fibbinary(idx_t n)
{
    uint64_t f[92];
    int k, i;
    uint64_t rem = (uint64_t)n;
    seq_t result = 0;

    f[0] = 1;
    f[1] = 2;
    k = 2;
    while (k < 92 && f[k - 1] <= rem) {
        f[k] = f[k - 1] + f[k - 2];
        k++;
    }
    for (i = k - 1; i >= 0; i--) {
        result = result * 2;
        if (f[i] <= rem) {
            rem -= f[i];
            result = result + 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t m = A345201_fibbinary(n);
    if (m == 0) return 0;

    unsigned tz = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++tz;
    }

    seq_t r = 0;
    while (m != 0) {
        r = (r << 1) | (m & 1);
        m >>= 1;
    }
    r <<= tz;

    seq_t res = 0;
    seq_t fa = 1, fb = 2;
    while (r != 0) {
        if ((r & 1) != 0) res += fa;
        seq_t t = fa + fb;
        fa = fb;
        fb = t;
        r >>= 1;
    }
    return res;
}
