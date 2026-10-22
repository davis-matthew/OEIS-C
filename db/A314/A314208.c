/*
 * A314208  Coordination sequence Gal.6.619.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A314208: Coordination sequence Gal.6.619.6
 *
 *  The differences between successive terms form a repeating
 *  12‑cycle after the initial step:
 *      d(1) = 4
 *      d(2..13) = { 6,6,5,5,5,5,5,5,6,6,5,5 }
 *
 *  Hence for n ≥ 2 the differences repeat every 12 indices.
 *
 *  Let diffCycle[12] be the 12‑term cycle starting at n = 2.
 *  Then for any n ≥ 2:
 *
 *      a(n) = 5 + 64 * floor((n-1)/12) + sum_{i=0}^{r-1} diffCycle[i],
 *          where r = (n-1) mod 12.
 *
 *  This formula reproduces all known terms and can be evaluated in
 *  constant time for any n ≥ 0.  Intermediate computations are
 *  performed in unsigned __int128 to guarantee correctness for
 *  all sequences that fit in the chosen seq_t type.
 */
#include <stdint.h>

static const seq_t diffCycle[12] = {6,6,5,5,5,5,5,5,6,6,5,5};
static const seq_t prefix[13] = {
    0,  6,  12,  17,  22,  27,  32,  37,  42,  48,  54,  59,  64
};

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;
    if (n == 1U)
        return (seq_t)5;

    idx_t count = n - 1U;            // number of differences considered
    idx_t full  = count / 12U;       // complete cycles
    idx_t rem   = count % 12U;       // remaining part of the cycle

    unsigned __int128 result = (unsigned __int128)full * (unsigned __int128)64;
    result += (unsigned __int128)prefix[rem] + (unsigned __int128)5;

    return (seq_t)result;
}
