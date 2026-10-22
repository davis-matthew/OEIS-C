/*
 * A345091  a(n) = Sum_{k=1..n} k^floor(1/gcd(n,k)).
 * https://oeis.org/A345091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n-2)*phi(n)/2 (for n >= 2), a(1) = 1
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    seq_t result = (seq_t)n;
    uint64_t tmp = n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) {
        result -= result / tmp;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t phi_n = phi_uint64((uint64_t)n);
    seq_t nn = (seq_t)n;
    return nn + ((nn - 2) * phi_n) / 2;
}
