/*
 * A369979  Three-dimensional array giving all products of three (not
 *          necessarily distinct) odd primes.
 * https://oeis.org/A369979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 363, u32 n <= 8632, u64 n <= 8632,
 *            u128 n <= 8632, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369979.
   a(n) is the n‑th product of three odd primes,
   enumerated by weakly decreasing triplets (x,y,z) with x>=y>=z>=1
   in lexicographical order.  The first term a(1) is 3*3*3 = 27,
   the second 5*3*3 = 45, etc.
*/

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) -> k‑th prime (2,3,5,7,…) */

static inline seq_t
odd_prime(idx_t i)            /* i=1 -> 3, i=2 -> 5, … */
{
    return get_prime(i + 1);
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of offset, safe default */

    /* Find minimal k such that the number of triplets
       with x <= k >= n.  Count = k*(k+1)*(k+2)/6. */
    idx_t k = 0;
    while ((k * (k + 1) * (k + 2)) / 6 < n)
        ++k;

    idx_t cnt = 0;
    for (idx_t x = 1; x <= k; ++x) {
        for (idx_t y = 1; y <= x; ++y) {
            for (idx_t z = 1; z <= y; ++z) {
                ++cnt;
                if (cnt == n) {
                    seq_t p1 = odd_prime(x);
                    seq_t p2 = odd_prime(y);
                    seq_t p3 = odd_prime(z);
                    return p1 * p2 * p3;
                }
            }
        }
    }
    return 0;   /* should never reach here */
}
