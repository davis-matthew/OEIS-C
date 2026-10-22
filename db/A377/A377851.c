/*
 * A377851  Smallest multiplier which can complete the square for n-polygonal
 *          numbers, together with a constant offset.
 * https://oeis.org/A377851
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n-2)/gcd(n,4)^2
 * Range:     u8 n <= 33, u16 n <= 8193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t n_seq = (seq_t)n;
    seq_t numerator = 8 * (n_seq - 2);
    seq_t g = gcd(n_seq, (seq_t)4);
    seq_t denom = g * g;
    return numerator / denom;
}
