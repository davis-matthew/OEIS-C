/*
 * A324127  Partial sums of A175046.
 * https://oeis.org/A324127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} A175046(k), where A175046(k) is k's binary
 *            expansion with each run lengthened by one bit
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A175046_a(idx_t n)
{
    seq_t result = 0;
    idx_t pos = 63;
    while (pos > 0 && !((n >> pos) & 1ULL))
        --pos;

    idx_t i = pos;
    while (1) {
        unsigned bit = (unsigned)((n >> i) & 1ULL);
        idx_t run = 1;
        while (i > 0 && ((n >> (i - 1)) & 1ULL) == bit) {
            ++run;
            --i;
        }
        for (idx_t k = 0; k <= run; ++k) {
            result = (result << 1) | (seq_t)bit;
        }
        if (i == 0)
            break;
        --i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t k = 1; k <= n; ++k)
        s += A175046_a(k);
    return s;
}
