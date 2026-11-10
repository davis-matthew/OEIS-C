/*
 * A334596  Number of values in A334556 with binary length n.
 * https://oeis.org/A334596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; a(n) = 0 if n-1 is a power of 2 (n = 2^k+1); otherwise
 *            a(n) = 2^A008611(n-4), where A008611(m) = m/3 + 1 if m%3 != 1,
 *            else m/3
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 96, u64 n <= 192,
 *            u128 n <= 384, big any n
 * Validated: standard, pass, 72 terms through n = 966 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A008611_a(idx_t n)
{
    idx_t k = n / 3;
    int r = (int)(n % 3);
    if (r == 1)
        return (seq_t)k;
    return (seq_t)(k + 1);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    idx_t m = n - 1;
    if ((m & (m - 1)) == 0)
        return (seq_t)0;
    idx_t e = (idx_t)A008611_a(n - 4);
    seq_t r = 1;
    for (idx_t i = 0; i < e; i++)
        r = r * 2;
    return r;
}
