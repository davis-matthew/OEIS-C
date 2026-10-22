/*
 * A356352  a(n) = GCD of run lengths in binary expansion of n.
 * https://oeis.org/A356352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = GCD of run lengths in the binary representation of n
 * Range:     u8 n <= 9276, u16 n <= 9276, u32 n <= 9276, u64 n <= 9276,
 *            u128 n <= 9276, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* binary expansion of 0 has no runs */
        return 0;

    /* current_bit holds the bit value of the run we are counting */
    unsigned current_bit = n & 1;
    seq_t run_len = 1;      /* length of the current run */
    seq_t gcd_val = 0;      /* GCD of finished runs (0 indicates uninitialized) */

    /* shift n right to process remaining bits */
    n >>= 1;

    while (n != 0) {
        unsigned bit = n & 1;
        if (bit == current_bit) {
            ++run_len;        /* same run continues */
        } else {
            /* run ends; update GCD with its length */
            gcd_val = gcd(gcd_val, run_len);
            run_len = 1;     /* start new run */
            current_bit = bit;
        }
        n >>= 1;
    }

    /* finish the last run */
    gcd_val = gcd(gcd_val, run_len);
    return gcd_val;
}
