/*
 * A280916  Number of dashes in International Morse numeral representation of
 *          n.
 * https://oeis.org/A280916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum over decimal digits d of |5 - d| (for n = 0 returns
 *            5). This follows from the recurrence a(n) = a(floor(n/10)) + a(n
 *            % 10) for n > 9, with a(d) = |5-d| for 0 <= d <= 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for zero: the digit 0 contributes 5 dashes. */
    if (n == (idx_t)0) return (seq_t)5;

    seq_t sum = (seq_t)0;
    idx_t temp = n;
    const idx_t TEN = (idx_t)10;

    while (temp > (idx_t)0) {
        idx_t digit = temp % TEN;
        seq_t diff  = (digit <= (idx_t)5)
            ? (seq_t)((idx_t)5 - digit)
            : (seq_t)(digit - (idx_t)5);
        sum += diff;
        temp /= TEN;
    }
    return sum;
}
