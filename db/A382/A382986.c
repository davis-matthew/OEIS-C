/*
 * A382986  a(n) is the number of iterations that n requires to reach 0 under
 *          the map k -> b(k) where b(k) = k+1 if k is even, and b(k) =
 *          k-gpf(k) if k is odd, where gpf(k) is the greatest prime dividing
 *          k.
 * https://oeis.org/A382986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations needed for n to reach 0 under
   the map:
       b(k) = k + 1,                     if k is even
       b(k) = k - gpf(k),                if k is odd
   where gpf(k) is the greatest prime factor of k.  This implements
   OEIS sequence A382986. */
#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* a(0) is 0 by definition */
    uint64_t k = (uint64_t)n;
    seq_t count = 0;

    while (k != 0) {
        ++count;
        if ((k & 1ULL) == 0) {        /* even */
            k = k + 1ULL;
        } else {                      /* odd */
            if (k == 1ULL) {
                k = 0;
            } else {
                k = k - gpf(k);
            }
        }
    }
    return count;
}
