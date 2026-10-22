/*
 * A342768  a(n) = A342767(n, n).
 * https://oeis.org/A342768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 / GPF(n) where GPF is the greatest prime factor. a(1)
 *            = 1. (GPF is obtained from the utility function gpf.)
 * Range:     u8 n <= 15, u16 n <= 484, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n <= 1)                   // a(1) = 1
        return 1;

    uint64_t g = gpf((uint64_t)n);        // greatest prime factor of n
    seq_t nn = (seq_t)n * (seq_t)n;        // compute n^2 in seq_t
    return nn / (seq_t)g;                  // divide by the GPF
}
