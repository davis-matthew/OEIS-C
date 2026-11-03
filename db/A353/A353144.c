/*
 * A353144  Decimal repunits written in base 5.
 * https://oeis.org/A353144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007091(A002275(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset 0: a(0) = 0.  Computes the decimal number obtained by
   interpreting the decimal repunit of length n as a base‑5 number
   and then reading the base‑5 digits as decimal digits. */

#include <stddef.h>
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t rep_to_a(seq_t rep)
{
    /* Convert rep (in decimal) to base-5 digits and assemble the
       corresponding decimal number.  rep > 0 for n > 0; rep == 0
       for n == 0 and yields a(n) == 0. */
    if (rep == 0)
        return 0;

    unsigned char digits[20000];
    size_t len = 0;
    while (rep != 0) {
        seq_t d = rep % ((seq_t)5);
        digits[len++] = (unsigned char)d;
        rep = rep / ((seq_t)5);
    }

    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        result = result * 10 + digits[i - 1];
    }
    return result;
}

seq_t a(idx_t n)
{
    /* repunit(n) = (10^n – 1) / 9  (for n = 0 gives 0) */
    seq_t rep = pow10(n);
    rep = rep - 1;
    rep = rep / 9;
    return rep_to_a(rep);
}
