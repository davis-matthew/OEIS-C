/*
 * A310480  Coordination sequence Gal.5.260.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence of Gal.5.260.1 (OEIS A310480).
 * For n ≥ 6 the sequence satisfies:
 *   if n ≡ 0 (mod 4) : a(n) = 5 n
 *   if n ≡ 1 (mod 4) : a(n) = 5 n + 1
 *   if n ≡ 2 (mod 4) : a(n) = 6 n – 4
 *   if n ≡ 3 (mod 4) : a(n) = 5 n + 3
 * Small values (n = 0…5) are handled explicitly to match the
 * first terms listed in the OEIS entry.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    switch ((n & 3ULL)) {
    case 0: /* n ≡ 0 (mod 4) */
        if (n == 0)          return (seq_t)1;     /* a(0) = 1 */
        if (n == 4)          return (seq_t)18;    /* a(4) = 18 */
        return (seq_t)n * (seq_t)5;               /* a(n) = 5 n  */
    case 1: /* n ≡ 1 (mod 4) */
        if (n == 1)          return (seq_t)4;     /* a(1) = 4 */
        if (n == 5)          return (seq_t)24;    /* a(5) = 24 */
        return (seq_t)n * (seq_t)5 + (seq_t)1;    /* a(n) = 5 n + 1 */
    case 2: /* n ≡ 2 (mod 4) */
        if (n == 2)          return (seq_t)10;    /* a(2) = 10 */
        return (seq_t)n * (seq_t)6 - (seq_t)4;    /* a(n) = 6 n – 4 */
    case 3: /* n ≡ 3 (mod 4) */
        if (n == 3)          return (seq_t)16;    /* a(3) = 16 */
        return (seq_t)n * (seq_t)5 + (seq_t)3;    /* a(n) = 5 n + 3 */
    }
    /* control never reaches here */
    return (seq_t)0;
}
