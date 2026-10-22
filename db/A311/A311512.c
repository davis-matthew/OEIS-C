/*
 * A311512  Coordination sequence Gal.6.130.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n ≥ 2, a(n)=a(n‑1)+4+Δ(n) where Δ(n) = 1 if
 *            n≡0 or 1 (mod 4) **and** floor(n/4)≢0 (mod 3), otherwise Δ(n)=0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies the types idx_t and seq_t.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small‑index base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t value = (seq_t)4;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t inc = (seq_t)4;        /* default increment */

        /* Check the pattern for Δ(i)=1. */
        if ( ((i & 3) <= 1) && (((i >> 2) % 3) != 0) )
            inc = (seq_t)5;

        value += inc;
    }
    return value;
}
