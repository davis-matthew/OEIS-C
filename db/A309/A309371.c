/*
 * A309371  a(n) = Sum_{k=1..n} k * A088370(n,k).
 * https://oeis.org/A309371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * A088370(n,k), where row n of A088370 is
 *            defined by T(1)={1}; T(r) = concat(2x-1 for x in T(ceil(r/2)),
 *            2x for x in T(floor(r/2)))
 * Range:     u8 n <= 9, u16 n <= 62, u32 n <= 2578, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9183 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t
A088370_get_val(idx_t r, idx_t p)
{
    if (r <= 1)
        return 1;
    idx_t k = (r + 1) >> 1;
    if (p <= k)
        return 2 * A088370_get_val(k, p) - 1;
    return 2 * A088370_get_val(r - k, p - k);
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t p;
    for (p = 1; p <= n; p++) {
        s += (seq_t)p * (seq_t)A088370_get_val(n, p);
    }
    return s;
}
