/*
 * A258438  Sum_{i=1..n} Sum_{j=1..n} (i OR j), where OR is the binary logical
 *          OR operator.
 * https://oeis.org/A258438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1..n} Σ_{j=1..n} (i OR j)
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1762, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;

    /* Handle bits 0 to 62 */
    for (unsigned int b = 0; ; ++b)
    {
        idx_t bit64 = (idx_t)1ULL << b;        /* 64‑bit intermediate for comparison */
        if (bit64 > n) break;                 /* no more bits set in any number 1..n */

        seq_t bit = (seq_t)bit64;             /* cast to seq_t after check */

        idx_t period = bit64 << 1;            /* 2^(b+1), no overflow for b<63 */
        idx_t full = n / period;
        idx_t cnt1 = full * bit64;
        idx_t rem   = n % period;
        if (rem >= bit64)
            cnt1 += rem - bit64 + 1;

        idx_t cnt0 = n - cnt1;

        seq_t contrib = ((seq_t)n * n - (seq_t)cnt0 * cnt0) * bit;
        sum += contrib;
    }

    /* Handle bit 63 separately (no shift of 64 bits) */
    idx_t bit63 = (idx_t)1ULL << 63;
    if (n >= bit63)
    {
        seq_t bit = (seq_t)bit63;
        seq_t cnt1 = (seq_t)n - bit + 1;
        seq_t cnt0 = (seq_t)n - cnt1;
        seq_t contrib = ((seq_t)n * n - cnt0 * cnt0) * bit;
        sum += contrib;
    }

    return sum;
}
