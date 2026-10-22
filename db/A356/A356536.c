/*
 * A356536  a(n) = Sum_{k=1..n} sigma_3(k)^2.
 * https://oeis.org/A356536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} sigma_3(k)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 30, u64 n <= 728,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = sigma_k(i, 3);
        sum += s * s;
    }
    return sum;
}
