/*
 * A332448  a(n) = A007814(A087808(sigma(n))).
 * https://oeis.org/A332448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A087808(sigma(n))), where A087808(0)=0,
 *            A087808(2k)=2*A087808(k), A087808(2k+1)=A087808(k)+1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9883 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000203_a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

static seq_t A087808_a(seq_t n)
{
    if (n == 0)
        return (seq_t)0;
    if ((n & 1) == 0)
        return (seq_t)2 * A087808_a(n >> 1);
    else
        return A087808_a(n >> 1) + (seq_t)1;
}

static seq_t A007814_a(seq_t n)
{
    seq_t count = 0;
    if (n == 0)
        return count;
    while ((n & 1) == 0) {
        count++;
        n >>= 1;
    }
    return count;
}

seq_t a(idx_t n)
{
    return A007814_a(A087808_a(A000203_a(n)));
}
