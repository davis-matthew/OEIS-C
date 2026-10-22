/*
 * A323606  Decimal expansion of 25146/15625.
 * https://oeis.org/A323606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = decimal digit of 25146/15625 at position n (offset 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined, return 0 for safety */

    const seq_t num = 25146;         /* numerator of the fraction */
    const seq_t den = 15625;         /* denominator */

    if (n == 1)
        return num / den;            /* integer part (1) */

    seq_t rem = num % den;           /* remainder after integer part */
    seq_t last = 0;

    for (idx_t i = 2; i <= n; ++i) {
        rem *= 10;                   /* bring down next decimal digit */
        last = rem / den;            /* digit at this position */
        rem %= den;                  /* new remainder */
    }

    return last;
}
