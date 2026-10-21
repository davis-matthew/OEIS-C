/*
 * A249123  Position of n^6 in the ordered union of {h^6, h >= 1} and {2*k^6,
 *          k >= 1}.
 * https://oeis.org/A249123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow6_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + floor(2^(-1/6) * n)
 *
 * Computes the position of n^6 in the ordered union of {h^6} and {2*k^6}.
 * Uses integer arithmetic only; no floating‑point or external helpers.
 */

#include <stdint.h>
#include "../utils/pow6_u128.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* compute high bound (n) */
    idx_t low  = 0;
    idx_t high = n;

    /* pre‑compute n^6 once */
    unsigned __int128 n6 = pow6_u128((uint64_t)n);

    /* binary search for largest k with 2*k^6 <= n^6 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 m6 = pow6_u128((uint64_t)mid);
        if ((unsigned __int128)2 * m6 <= n6)
            low = mid;          /* mid satisfies inequality */
        else
            high = mid - 1;     /* mid too large */
    }

    /* a(n) = n + floor(n / 2^(1/6)) = n + low */
    return (seq_t)n + (seq_t)low;
}
