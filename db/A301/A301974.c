/*
 * A301974  Decimal expansion of value of the Fermi coupling constant G_F in
 *          GeV^(-2).
 * https://oeis.org/A301974
 *
 * Offset:    -5    (a(-5) is the first term)
 * Keywords:  nonn, cons, more
 * Formula:   not stated
 * Range:     u8 n <= -4, u16 n <= -4, u32 n <= 0, u64 n <= 0, u128 n <= 0,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A301974: decimal digits of the Fermi coupling constant
   G_F = 1.1663787 ×10^(-5) GeV^(-2).  The sequence yields the
   decimal digits of 1.1663787 (with the integer part first) and
   all subsequent digits after the seventh decimal are zero.
   Offset: -5.  Thus a(-5) = 1 (integer part), a(-4)=1, a(-3)=6, … */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < -5) return 0;          /* behaviour below the offset is irrelevant */
    /* position in the digit sequence, 0 for integer part */
    idx_t pos = n + 5;
    const seq_t NUM = (seq_t)11663787;   /* 1.1663787 × 10^7 */
    const seq_t DEN = (seq_t)10000000;   /* 10^7 */

    if (pos == 0) return NUM / DEN;      /* integer part = 1 */

    seq_t remainder = NUM % DEN;
    for (idx_t i = 1; i <= pos; ++i) {
        remainder *= 10;
        seq_t digit = remainder / DEN;
        remainder %= DEN;
        if (i == pos) return digit;
    }
    return 0; /* unreachable */
}
