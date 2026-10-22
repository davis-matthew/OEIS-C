/*
 * A301755  Decimal expansion of 3/8.
 * https://oeis.org/A301755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = digit n of the decimal expansion of 3/8 (0-indexed:
 *            a(0)=3, a(1)=7, a(2)=5, a(n≥3)=0). Implemented via long‑division
 *            of the fraction 3/8 at position n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* start with the numerator 3 (the integer part is 0); */
    seq_t rem = 3;          /* remainder before each digit extraction */
    seq_t digit;

    /* perform the long division digit by digit up to position n */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                  /* bring down a zero */
        digit = rem / 8;            /* next decimal digit */
        rem   = rem % 8;            /* new remainder */
        if (i == n) {
            return digit;           /* desired digit found */
        }
    }

    /* This point is unreachable but required for a return statement. */
    return 0;
}
