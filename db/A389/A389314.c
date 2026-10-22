/*
 * A389314  Decimal expansion of 1/3432.
 * https://oeis.org/A389314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n in the decimal expansion of 1/3432
 *            (a(0) is the first digit after the decimal point)
 * Range:     u8 n <= 1002, u16 n <= 1002, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The expansion has period 6 after the third digit, but this implementation
   recomputes the digit directly by linear‐time remainder iteration,
   which satisfies the performance and correctness requirements. */

seq_t a(idx_t n)
{
    /* start with the numerator 1 and iterate the division process */
    idx_t r = 1;                     /* current remainder, 0 <= r < 3432 */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;                  /* bring down the next decimal place */
        seq_t digit = (seq_t)(r / 3432); /* the quotient is the next digit */
        r = r % 3432;                 /* remainder for the next iteration */
        if (i == n) return digit;    /* return the n‑th digit when reached */
    }
    return 0; /* unreachable, but keeps the compiler happy */
}
