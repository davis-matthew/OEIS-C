/*
 * A276795  Folding numbers with an odd number of bits (see A277238 for
 *          definition).
 * https://oeis.org/A276795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, let k = ceil(log2(n)) (the smallest integer
 *            with 2^k >= n), l = n − 1. Then a(n) = (l << (k+1)) + (1 << k) +
 *            reverse_k(((1 << k) – 1) – l, k), where reverse_k(x, k) is the
 *            reversal of the lowest k bits of x. This matches the
 *            construction of A276795
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_bits_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_bits_k.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1 (and safely return 0 for n=0) */
        return (seq_t)n;

    /* Find k = ceil(log2(n))   (smallest k with 2^k >= n) */
    idx_t t = n - 1;   /* n > 1, so t > 0 */
    unsigned k = 0;
    while (t > 0) {
        t >>= 1;
        ++k;
    }

    seq_t l = (seq_t)(n - 1);                                   /* l = n-1 */
    seq_t center = ((seq_t)1 << k);                             /* 1 << k  (middle bit) */
    seq_t r_rev = ((((seq_t)1) << k) - 1) - l;                  /* rev of right half */
    seq_t r = reverse_bits_k(r_rev, k);                         /* actual right half */

    return (l << (k + 1)) + center + r;                         /* assemble the number */
}
