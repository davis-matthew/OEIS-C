/*
 * A348416  For n >= 1; a(n) = gcd(n,w(n)) where w(n) is the binary weight of
 *          n, A000120(n).
 * https://oeis.org/A348416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = gcd(n, popcount(n)) where popcount(n) is the binary
 *            weight of n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cast to seq_t to use the generic gcd implementation */
    seq_t sqn = (seq_t)n;
    /* popcount for 64‑bit integers; result fits in an int */
    seq_t w = (seq_t)__builtin_popcountll(n);
    return gcd(sqn, w);
}
