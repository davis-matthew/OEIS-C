/*
 * A353669  a(n) = 1 if n is a nonsquare that has an even number of prime
 *          factors with multiplicity, otherwise 0.
 * https://oeis.org/A353669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a non‑square and the number of prime factors of
 *            n with multiplicity (bigomega(n)) is even, otherwise 0. Index 1
 *            is the first term
 * Range:     u8 n <= 9543, u16 n <= 9543, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper bigomega(idx_t) from ../utils/bigomega.h. */
#include "../utils/bigomega.h"

static idx_t floor_sqrt_u64(idx_t n)
{
    /* binary search for ⌊√n⌋, using 128‑bit multiplication to avoid overflow */
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    idx_t s = floor_sqrt_u64(n);
    unsigned __int128 sq = (unsigned __int128)s * s;
    if (sq == n)                    /* perfect square → 0 */
        return 0;

    seq_t omega = bigomega(n);      /* number of prime factors with multiplicity */
    return ((omega & 1U) == 0) ? 1 : 0;
}
