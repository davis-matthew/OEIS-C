/*
 * A379118  Characteristic function of A379114, numbers k that have a proper
 *          unitary divisor d > 1 such that
 *          A048720(A065621(sigma(d)),sigma(k/d)) is equal to sigma(k).
 * https://oeis.org/A379118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if some proper unitary divisor d>1 of n has
 *            A048720(A065621(sigma(d)), sigma(n/d)) = sigma(n), else 0; a(n)
 *            = [A379113(n) > 1]
 * Range:     u8 n <= 89, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9819 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A379118_rev(seq_t m)
{
    /* A065621: m XOR ((m - lowbit(m)) << 1) */
    seq_t low = 1;
    if (m == 0) return 0;
    while ((m & low) == 0) low <<= 1;
    return m ^ ((m - low) << 1);
}

static seq_t A379118_gf2mul(seq_t i, seq_t j)
{
    seq_t result = 0;
    seq_t shifted = i;
    while (j != 0) {
        if ((j & 1) != 0) result ^= shifted;
        shifted <<= 1;
        j >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sg[64];
    int cnt = 0;
    idx_t m = n;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t term = 1, sum = 1;
            while (m % p == 0) {
                m /= p;
                term *= (seq_t)p;
                sum += term;
            }
            sg[cnt++] = sum;
        }
    }
    if (m > 1) {
        sg[cnt++] = (seq_t)1 + (seq_t)m;
    }
    if (cnt < 2) return 0;
    seq_t total = 1;
    int i;
    for (i = 0; i < cnt; i++) total *= sg[i];
    unsigned long full = (1UL << cnt) - 1;
    unsigned long mask;
    for (mask = 1; mask < full; mask++) {
        seq_t sd = 1, sr = 1;
        for (i = 0; i < cnt; i++) {
            if ((mask >> i) & 1UL) sd *= sg[i];
            else sr *= sg[i];
        }
        if (A379118_gf2mul(A379118_rev(sd), sr) == total) return 1;
    }
    return 0;
}
