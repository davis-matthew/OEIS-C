/*
 * A257588  If n = abcd... in decimal, a(n) = |a^2 - b^2 + c^2 - d^2 + ...|.
 * https://oeis.org/A257588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |a^2 - b^2 + c^2 - d^2 + ...|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the alternating sum of squares of decimal digits of n,  */
seq_t a(idx_t n)
{
    signed long long sum = 0;   /* signed intermediate accumulator  */
    int sign = 1;              /* +1 / -1 for alternating signs     */
    idx_t tmp = n;

    while (tmp > 0) {
        unsigned digit = (unsigned)(tmp % 10);  /* extract last digit   */
        unsigned sq = digit * digit;           /* square it (max 81)   */
        sum += sign * (signed long long)sq;    /* add with current sign*/
        sign = -sign;                          /* flip sign            */
        tmp /= 10;                             /* move to next digit   */
    }

    if (sum < 0) sum = -sum;                   /* absolute value       */
    return (seq_t)sum;
}
