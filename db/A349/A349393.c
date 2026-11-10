/*
 * A349393  Inverse Möbius transform of A126760.
 * https://oeis.org/A349393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A126760(d), where A126760(2n)=A126760(n),
 *            A126760(3n)=A126760(n), A126760(6n+1)=2n+1, A126760(6n+5)=2n+2
 * Range:     u8 n <= 594, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9676 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A126760_a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t r = m % 6;
        if (r == 1) {
            result = ((seq_t)2 * (seq_t)((m - 1) / 6)) + 1;
            break;
        }
        if (r == 5) {
            result = ((seq_t)2 * (seq_t)((m - 5) / 6)) + 2;
            break;
        }
        if ((m & 1) == 0) {
            m >>= 1;
            continue;
        }
        if (m % 3 == 0) {
            m /= 3;
            continue;
        }
        break;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            idx_t j = n / i;
            sum += A126760_a(i);
            if (j != i) sum += A126760_a(j);
        }
    }
    return sum;
}
