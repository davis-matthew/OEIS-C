/*
 * A367547  Euclid's triangle A217831 represented as binary numbers.
 * https://oeis.org/A367547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the binary string whose j‑th digit (1 ≤
 *            j ≤ n, j = 1 is the most significant digit) is 1 iff gcd(j,n) =
 *            1. For n = 0 output 0; for n = 1 output 11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;
    }
    if (n == (idx_t)1) {
        return (seq_t)11;
    }
    seq_t res = (seq_t)0;
    for (idx_t j = (idx_t)1; j <= n; ++j) {
        seq_t bit = (gcd_u64(j, n) == (idx_t)1) ? (seq_t)1 : (seq_t)0;
        res = res * (seq_t)10 + bit;
    }
    return res;
}
