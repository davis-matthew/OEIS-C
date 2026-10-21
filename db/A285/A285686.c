/*
 * A285686  Characteristic sequence of the Beatty sequence, A022840, of
 *          sqrt(6).
 * https://oeis.org/A285686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/sqrt(6)) - floor(n/sqrt(6))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_div6_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using integer arithmetic only, no floating point. */

#include <stdint.h>
#include <limits.h>
#include "../utils/floor_sqrt_div6_u64.h"

/* Characteristic sequence A285686. */
seq_t a(idx_t n)
{
    seq_t k1 = floor_sqrt_div6_u64(n);
    seq_t k2 = floor_sqrt_div6_u64((idx_t)(n + 1));
    return k2 - k1;
}
