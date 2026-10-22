/*
 * A327211  a(n) = [(2n+2)e] - [(n+2)e] - [ne], where [ ] = floor.
 * https://oeis.org/A327211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+2)e) - floor((n+2)e) - floor(ne)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_ne
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>
#include "../utils/floor_ne.h"

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate 2*n+2 calculation
       to avoid overflow when n is large. */
    unsigned __int128 tmp = (unsigned __int128)n * 2 + 2;
    idx_t n2 = (idx_t)tmp;

    return floor_ne(n2) - floor_ne(n + 2) - floor_ne(n);
}
