/*
 * A376507  Natural numbers whose iterated squaring modulo 100 eventually
 *          settles at the attractor 76.
 * https://oeis.org/A376507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A376507 – Natural numbers whose iterated squaring modulo 100
 *  eventually settles at 76.
 *  The sequence consists of all numbers congruent to 18, 24, 26 or 32
 *  modulo 50.  The terms can be written explicitly as:
 *
 *      a(n) = 18 + 50*((n-1)/4)   for n ≡ 1 (mod 4)
 *      a(n) = 24 + 50*((n-2)/4)   for n ≡ 2 (mod 4)
 *      a(n) = 26 + 50*((n-3)/4)   for n ≡ 3 (mod 4)
 *      a(n) = 32 + 50*((n-4)/4)   for n ≡ 0 (mod 4)
 *
 *  This formula reproduces the OEIS table and the recurrence of the
 *  first‑order differences (6, 2, 6, 36, …) dropping into a period‑4
 *  pattern.  The implementation below performs the computation in
 *  constant time, using only integer arithmetic compatible with C11
 *  and C++17.
 */

#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* n is 1‑based; the function is undefined for n == 0. */
    idx_t  rem = n % 4;
    seq_t  base;
    idx_t  k;

    switch (rem) {
        case 0:       /* n ≡ 0 (mod 4) */
            base = (seq_t)32;
            k    = (n - 4) / 4;   /* integer division, safe because n≥4 here */
            break;
        case 1:       /* n ≡ 1 (mod 4) */
            base = (seq_t)18;
            k    = (n - 1) / 4;
            break;
        case 2:       /* n ≡ 2 (mod 4) */
            base = (seq_t)24;
            k    = (n - 2) / 4;
            break;
        default:      /* rem == 3, n ≡ 3 (mod 4) */
            base = (seq_t)26;
            k    = (n - 3) / 4;
            break;
    }
    return base + (seq_t)50 * k;
}
