/*
 * A395992  Decimal expansion of 20612/6561.
 * https://oeis.org/A395992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit of the decimal expansion of 20612/6561
 *            (a(1) = integer part 3, a(2) = first fractional digit, …)
 * Range:     u8 n <= 1, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants in the sequence's definition */
    const seq_t NUM = (seq_t)20612u;
    const seq_t DEN = (seq_t)6561u;

    /* a(1) is the integer part of the fraction */
    if (n == 1) {
        return (seq_t)3u;
    }

    /* position in the fractional part (0‑based) */
    idx_t pos = n - 1;

    /* current remainder; initially NUM % DEN */
    seq_t rem = NUM % DEN;

    seq_t digit = 0;

    for (idx_t i = 0; i < pos; ++i) {
        rem = rem * (seq_t)10u;   /* shift left by one decimal place */
        digit = rem / DEN;        /* next digit (0–9) */
        rem = rem % DEN;          /* new remainder for next step   */
    }

    return digit;
}
