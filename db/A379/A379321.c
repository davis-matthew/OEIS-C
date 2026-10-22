/*
 * A379321  Decimal expansion of 8405139762/2675439081.
 * https://oeis.org/A379321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal expansion of 8405139762/2675439081 (including integer
 *            part)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The fraction 8405139762/2675439081 gives an excellent rational
       approximation of π.  We return the nth decimal digit of this
       expansion, with a(1) equal to the integer part. */
    const unsigned long long NUM = 8405139762ULL;
    const unsigned long long DEN = 2675439081ULL;

    /* Integer part (first digit) */
    if (n == 1)
        return (seq_t)(NUM / DEN);

    /* Remainder after removing the integer part */
    unsigned long long rem = NUM % DEN;
    seq_t digit = 0;

    /* Compute digits using long division */
    for (idx_t i = 2; i <= n; ++i) {
        rem *= 10ULL;
        digit = (seq_t)(rem / DEN);
        rem %= DEN;
    }

    return digit;
}
