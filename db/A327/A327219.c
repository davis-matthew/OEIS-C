/*
 * A327219  a(n) = [(2n+4)r] - [(n+4)r] - [nr], where [ ] = floor and r =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A327219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(2n+4)φ⌋ – ⌊(n+4)φ⌋ – ⌊nφ⌋
 * Range:     u8 none, u16 n <= 26, u32 n <= 6703, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where φ = (1+√5)/2. The function works for any
 *          integer type seq_t (built‑in or arbitrary precision)
 *          and for any unsigned integer type idx_t.           */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Compute ⌊n·φ⌋ from the identity
 *   ⌊n·φ⌋ = ⌊(n + ⌊n·√5⌋)/2⌋
 *  where ⌊n·√5⌋ is obtained by taking the integer square root of 5·n².
 */
static inline seq_t floor_nphi(idx_t n)
{
    seq_t ns = (seq_t)n;              /* cast index to seq_t */
    seq_t t   = ns * ns;              /* n²  (seq_t) */
    t        = t * (seq_t)5;          /* 5·n² */
    seq_t r   = floor_sqrt(t);        /* ⌊√(5·n²)⌋ = ⌊n·√5⌋ */
    return (ns + r) / 2;              /* integer division gives the floor */
}

seq_t a(idx_t n)
{
    seq_t fn   = floor_nphi(n);
    seq_t fn4  = floor_nphi(n + 4);
    seq_t f2n4 = floor_nphi((n << 1) + 4);   /* 2n+4, shift is safe for idx_t */

    /* Each subtraction uses unsigned arithmetic – the result is 0 or 1. */
    return f2n4 - fn4 - fn;
}
