/*
 * A328559  Numbers m that are neither arithmetic (A003601) nor squares
 *          (A000290).
 * https://oeis.org/A328559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   divisor_count_u64, sigma_of, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th number m such that
   neither the arithmetic mean of the divisors of m
   (i.e. σ(m)/τ(m)) nor sqrt(m) is an integer.  */

/* Utilities used:
   - sqrt_floor:      seq_t sqrt_floor(seq_t x);
   - sigma_of:        seq_t sigma_of(idx_t n);
   - divisor_count_u64: size_t divisor_count_u64(uint64_t n);
*/
#include "../utils/sqrt_floor.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t m = 2; ; ++m)
    {
        /* skip squares */
        seq_t r = sqrt_floor((seq_t)m);
        if (r * r == (seq_t)m)
            continue;

        seq_t sigma = sigma_of(m);
        seq_t d = (seq_t)divisor_count_u64(m);
        if (sigma % d != 0)
        {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
    }
}
