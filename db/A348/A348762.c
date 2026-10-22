/*
 * A348762  a(n) = A000265((n-8)*(n+8)).
 * https://oeis.org/A348762
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000265((n-8)*(n+8))
 * Range:     u8 n <= 17, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = the largest odd divisor of (n-8)*(n+8) */

#include <stdint.h>  /* may be needed for the standard integer types */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 9.  For smaller n we return 0,
       although the caller is not expected to ask for those values. */
    if (n < 9) {
        return 0;
    }

    /* Compute (n-8)*(n+8) in seq_t, then strip factors of 2. */
    seq_t nm8 = (seq_t)n - (seq_t)8;
    seq_t np8 = (seq_t)n + (seq_t)8;
    seq_t x   = nm8 * np8;

    while ((x & (seq_t)1) == 0) {
        x >>= 1;
    }
    return x;
}
