/*
 * A372445  a(n) = A371092(A372443(n)).
 * https://oeis.org/A372445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A371092(A372443(n)) = floor((odd_part(3*x+1)+5)/6), where
 *            x = A372443(n); A372443: x(0) = 27, x(k) = odd_part(3*x(k-1)+1)
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-10)
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_part.h"

static seq_t A372445_R(seq_t x)
{
    return odd_part(x * 3 + 1);
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)27;
    for (idx_t i = 0; i < n; ++i)
        x = A372445_R(x);
    /* A371092(x) = floor((odd_part(3x+1)+5)/6) */
    seq_t y = A372445_R(x);
    return (y + 5) / 6;
}
