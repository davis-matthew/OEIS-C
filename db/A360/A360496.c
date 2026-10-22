/*
 * A360496  a(n) is the remainder after dividing n by its largest prime factor
 *          plus 1, a(1) = 1.
 * https://oeis.org/A360496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (1 + largest prime factor of n) (OEIS A360496)
 * Range:     u8 n <= 255, u16 n <= 6999, u32 n <= 6999, u64 n <= 6999,
 *            u128 n <= 6999, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n <= 1)
        return (seq_t)1;

    uint64_t lpf = gpf((uint64_t)n);          /* greatest prime factor of n */
    seq_t denom = (seq_t)(lpf + 1);           /* 1 + largest prime factor */
    seq_t numer = (seq_t)n;                   /* n as seq_t for modulo */

    return numer % denom;
}
