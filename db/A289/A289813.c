/*
 * A289813  A binary encoding of the ones in ternary representation of n (see
 *          Comments for precise definition).
 * https://oeis.org/A289813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 6560, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Validated: standard, pass, 98 terms through n = 6359 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   a(3*n)   = 2*a(n);
   a(3*n+1) = 2*a(n)+1;
   a(3*n+2) = 2*a(n); */

#include <stdint.h>

/* Recursive helper at file scope – must not be inside a function. */
static seq_t recur(idx_t x)
{
    if (x == 0)
        return 0;
    seq_t r = recur(x / 3);
    if (x % 3 == 1)
        return (r << 1) | 1;
    return r << 1;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    return recur(n);
}
