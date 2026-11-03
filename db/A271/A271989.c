/*
 * A271989  g_n(8) where g is the weak Goodstein function defined in A266202.
 * https://oeis.org/A271989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = b_{n+1}(a(n-1)) - 1, where a(0) = 8 and b_k(x)
 *            reinterprets the base-k representation of x as a number in base
 *            (k+1)
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for basic integer types */

/* reinterpret 'x' written in base 'base' as a number in base 'base+1' */
static seq_t reinterpret_base(seq_t x, idx_t base)
{
    seq_t base_seq  = (seq_t)base;
    seq_t new_base  = base_seq + 1;
    seq_t result    = 0;
    seq_t pow       = 1;

    while (x > 0) {
        seq_t digit = x % base_seq;
        x          = x / base_seq;
        result += digit * pow;
        pow    *= new_base;
    }
    return result;
}

/* public sequence function */
seq_t a(idx_t n)
{
    seq_t val = 8;                 /* a(0) = 8 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t new_val = reinterpret_base(val, i + 1);
        val = new_val - 1;
    }
    return val;
}
