/*
 * A268837  Antidiagonal sums of array A268715: a(n) = Sum_{k=0..n}
 *          A003188(A006068(n)+A006068(n-k)).
 * https://oeis.org/A268837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A003188(A006068(k)+A006068(n-k)), where
 *            A003188(m) = m XOR floor(m/2) and A006068 is the inverse Gray
 *            code
 * Range:     u8 n <= 12, u16 n <= 216, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Validated: standard, pass, 75 terms through n = 4015 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A003188_a(seq_t m)
{
    return m ^ (m >> 1);
}

static seq_t A006068_a(idx_t n)
{
    seq_t result = (seq_t)n;
    seq_t shifted = (seq_t)n;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        seq_t s = A006068_a(k) + A006068_a(n - k);
        sum += A003188_a(s);
    }
    return sum;
}
