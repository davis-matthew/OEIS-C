/*
 * A265335  Self-inverse permutation of nonnegative integers: a(n) =
 *          A264994(5*n)/5.
 * https://oeis.org/A265335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A264994(5*n)/5, where A264994(m) reverses the base-4
 *            digits of m with trailing zero digits kept at the end (a(4n) =
 *            4*a(n))
 * Range:     u8 n <= 51, u16 n <= 13107, u32 n <= 13107, u64 n <= 13107,
 *            u128 n <= 13107, big any n
 * Validated: standard, pass, 98 terms through n = 9505 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A265335_rev4(seq_t m)
{
    if (m == 0) return 0;
    unsigned k = 0;
    while ((m & 3) == 0) { m >>= 2; k++; }
    seq_t r = 0;
    while (m != 0) {
        r = r * 4 + (m & 3);
        m >>= 2;
    }
    while (k--) r = r * 4;
    return r;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 5;
    return A265335_rev4(m) / 5;
}
