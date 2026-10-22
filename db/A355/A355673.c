/*
 * A355673  Decimal expansion of 265/153.
 * https://oeis.org/A355673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th digit in the decimal expansion of 265/153
 * (the first term is the integer part, subsequent terms are the decimal digits).
 * It is computed by simulating long division:
 *  - a(1) = floor(265/153) = 1.
 *  - For n > 1, a(n) is obtained by repeated multiplication of the remainder
 *    by 10 and division by 153 to extract digits one by one.
 */
seq_t a(idx_t n)
{
    const seq_t NUM = (seq_t)265;
    const seq_t DEN = (seq_t)153;

    /* Integer part */
    seq_t integer_part = NUM / DEN;

    if (n == (idx_t)1)
        return integer_part;

    /* Start with the remainder after the integer part */
    seq_t r = NUM % DEN;
    idx_t k = n - (idx_t)1;       /* number of decimal digits to generate */
    seq_t digit = 0;

    for (idx_t i = 0; i < k; ++i) {
        r = r * (seq_t)10;
        digit = r / DEN;
        r = r % DEN;
    }

    return digit;
}
