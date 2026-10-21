/*
 * A274516  Place n equally-spaced points around a circle, labeled
 *          0,1,2,...,n-1. For each i = 0..n-1 such that 5i != i mod n, draw
 *          an (undirected) chord from i to (5i mod n). Then a(n) is the total
 *          number of distinct chords.
 * https://oeis.org/A274516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (gcd(n,4)+gcd(n,24))/2; a(0)=0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t g = gcd_idx(n, 4);
    seq_t h = gcd_idx(n, 24);
    return (seq_t)n - (g + h) / 2;
}
