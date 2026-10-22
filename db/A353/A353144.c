/*
 * A353144  Decimal repunits written in base 5.
 * https://oeis.org/A353144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007091(A002275(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pow10, rep_to_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset 0: a(0) = 0.  Computes the decimal number obtained by
   interpreting the decimal repunit of length n as a base‑5 number
   and then reading the base‑5 digits as decimal digits. */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/rep_to_a.h"

seq_t a(idx_t n)
{
    /* repunit(n) = (10^n – 1) / 9  (for n = 0 gives 0) */
    seq_t rep = pow10(n);
    rep = rep - 1;
    rep = rep / 9;
    return rep_to_a(rep);
}
