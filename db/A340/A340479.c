/*
 * A340479  a(n) = R(n) + digsum(n).
 * https://oeis.org/A340479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(n) + A007953(n) (reverse decimal of n plus digit
 *            sum of n)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;
    seq_t sum = 0;
    seq_t temp = (seq_t)n;      /* start from n, cast to seq_t for arithmetic */
    while (temp > 0) {
        seq_t digit = temp % 10;   /* last decimal digit */
        rev = rev * 10 + digit;    /* build the reverse number */
        sum += digit;              /* accumulate digit sum */
        temp /= 10;                /* drop the processed digit */
    }
    return rev + sum;
}
