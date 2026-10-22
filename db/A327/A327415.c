/*
 * A327415  Representation of integers by the product of prime partitions.
 * https://oeis.org/A327415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal product of parts of a prime partition of n as
 *            defined in OEIS A327415
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset cases */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* Even n > 2 : partition [p, n-p] where p is the greatest prime < n-1 */
    if ((n & 1) == 0) {
        for (idx_t p = n - 2; p >= 2; --p) {
            if (is_prime(p)) {
                seq_t prod = (seq_t)p * (seq_t)(n - p);
                return prod;
            }
        }
        /* Fallback – should never happen for n >= 4 */
        return (seq_t)n;
    }

    /* Odd n : if n is prime, the partition is [n]; otherwise
       use the largest odd prime p <= n and (n-p)/2 copies of 2 */
    if (is_prime(n)) {
        return (seq_t)n;
    } else {
        idx_t p;
        for (p = n - 2; p >= 3; p -= 2) { /* only odd candidates */
            if (is_prime(p)) break;
        }
        if (p < 3) p = 3; /* safety: for very small n */
        idx_t k = (n - p) / 2;
        seq_t prod = seq_pow((seq_t)2, k);
        prod *= (seq_t)p;
        return prod;
    }
}
