/*
 * A389443  a(n) is the number of ways n can be written as (w + x)*(y + z),
 *          where w, x, y and z are distinct positive integers with w < x, y <
 *          z, and w < y.
 * https://oeis.org/A389443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the OEIS sequence A389443.
   a(n) = number of ways n can be written as (w+x)*(y+z) with
   w,x,y,z positive integers, w<x, y<z, w<y and all four numbers distinct.
   The implementation enumerates all factor pairs of n and checks all
   suitable quadruples directly.  The algorithm runs in O(d(n)*sqrt(n))
   time, where d(n) is the number of divisors of n, comfortably fast
   for n up to around 10000.
*/
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n < 4) return 0; /* product of two sums >= 1+2=3 * 1+2=3 = 9, but
                            we simply return 0 for small n to avoid loops */
    /* Enumerate all divisors a of n up to sqrt(n) */
    for (idx_t a = 1; a * a <= n; ++a) {
        if (n % a != 0) continue;
        idx_t b = n / a;          /* pair (a,b) and possibly (b,a) */
        /* Process orientation (s1=a, s2=b) */
        {
            idx_t s1 = a, s2 = b;
            for (idx_t w = 1; w * 2 < s1; ++w) {
                idx_t x = s1 - w;
                /* w < x guaranteed by loop bound */
                for (idx_t y = 1; y * 2 < s2; ++y) {
                    idx_t z = s2 - y;
                    /* w < y required */
                    if (!(w < y)) continue;
                    /* All four numbers must be distinct */
                    if (w == y || w == z || x == y || x == z) continue;
                    /* w,x,y,z are all positive by construction */
                    count += (seq_t)1;
                }
            }
        }
        /* If a != b, also process swapped orientation (s1=b, s2=a) */
        if (a != b) {
            idx_t s1 = b, s2 = a;
            for (idx_t w = 1; w * 2 < s1; ++w) {
                idx_t x = s1 - w;
                for (idx_t y = 1; y * 2 < s2; ++y) {
                    idx_t z = s2 - y;
                    if (!(w < y)) continue;
                    if (w == y || w == z || x == y || x == z) continue;
                    count += (seq_t)1;
                }
            }
        }
    }
    return count;
}
