/*
 * A297787  Decimal expansion of 16968017/999^3.
 * https://oeis.org/A297787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 16968017/(999^3) =
 *            16968017/997002999. The sequence is obtained by performing long
 *            division of the fraction and taking the digit generated at step
 *            n
 * Range:     u8 n <= 0, u16 none, u32 n <= 1, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* 16968017/999^3  -> 16968017/997002999 */
    const seq_t numerator = 16968017;
    const seq_t denominator = 997002999;
    seq_t remainder = numerator;          /* first remainder (≡ numerator) */

    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        remainder *= (seq_t)10;           /* bring down next digit */
        digit = remainder / denominator; /* quotient digit */
        remainder = remainder % denominator;
    }
    return digit;                         /* the nth decimal digit (0‑based) */
}
