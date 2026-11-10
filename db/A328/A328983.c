/*
 * A328983  Complement of A328982.
 * https://oeis.org/A328983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for m>=1 the terms 5m-1-eps(m), 5m, 5m+1 in order, where
 *            eps(m)=1 if m has an odd number of trailing binary zeros, else 0
 *            (complement of A328982)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    idx_t j = n - 2;
    idx_t m = j / 3 + 1;
    idx_t r = j % 3;
    if (r == 1) return (seq_t)5 * (seq_t)m;
    if (r == 2) return (seq_t)5 * (seq_t)m + 1;
    unsigned tz = 0;
    idx_t x = m;
    while ((x & 1u) == 0u) {
        tz++;
        x >>= 1;
    }
    seq_t v = (seq_t)5 * (seq_t)m - 1;
    if (tz & 1u) v -= 1;
    return v;
}
