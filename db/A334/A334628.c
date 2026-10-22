/*
 * A334628  Total area of all distinct rectangles whose length and width are
 *          relatively prime and L + W = n.
 * https://oeis.org/A334628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} [gcd(i,n-i)=1] * i * (n-i)
 * Range:     u8 n <= 16, u16 n <= 96, u32 n <= 3726, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition; the loop will correctly return 0 for n==1. */
    seq_t total = 0;
    idx_t half = n / 2;

    for (idx_t i = 1; i <= half; ++i) {
        /* gcd(i, n) == 1 is equivalent to gcd(i, n-i) == 1. */
        if (gcd((seq_t)i, (seq_t)n) == 1) {
            seq_t prod = (seq_t)i * ((seq_t)n - (seq_t)i);
            total += prod;
        }
    }
    return total;
}
