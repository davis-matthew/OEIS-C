/*
 * A243356  Numbers n such that n and phi(n) have no common digits.
 * https://oeis.org/A243356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements the definition of OEIS sequence A243356.
   A(n) is the n-th positive integer with the property that
   n and φ(n) share no common decimal digits.  The function
   scans through the positive integers, computes φ(n) (using
   the provided helper calc_phi), extracts the digit sets of
   n and φ(n), and returns the n‑th number satisfying the
   condition.  Only byte‑stream I/O, the standard library and
   the supplied helper functions are used.  */

#include <stdbool.h>
#include "../utils/calc_phi.h"

/* For a given seq_t value, build a 10‑bit mask of the decimal
   digits that occur in it.  The least significant bit
   corresponds to digit 0. */
static inline unsigned int digit_mask(seq_t v)
{
    unsigned int mask = 0;
    while (v > 0) {
        unsigned int d = (unsigned)(v % 10);  /* v%10 fits in a small integer */
        mask |= 1u << d;
        v /= 10;
    }
    return mask;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined for n<1, return 0 as a safe sentinel. */
        return 0;

    seq_t current = 2;          /* First candidate (a(1)=2). */
    idx_t found  = 0;

    while (true) {
        seq_t phi = calc_phi(current);

        unsigned int mask1 = digit_mask(current);
        unsigned int mask2 = digit_mask(phi);

        /* Numbers share a digit iff the intersection of the masks is nonzero. */
        if ((mask1 & mask2) == 0) {
            ++found;
            if (found == n)
                return current;
        }
        ++current;
    }
}
