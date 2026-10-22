/*
 * A341513  Sum of digits in A097801-base.
 * https://oeis.org/A341513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of n in A097801-base where the radices are
 *            2, 3, 5, 7, 9, 11, 13, ... (i.e. 2 for the first position, and
 *            for position i ≥ 2 the radix is 2*i-1)
 * Range:     u8 n <= 1769, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cur = (seq_t)n;
    idx_t i = 1;                     /* first radix index (2) */
    while (cur != 0) {
        seq_t base = (i == 1) ? 2 : (seq_t)(2*i - 1);
        seq_t digit = cur % base;
        sum += digit;
        cur /= base;
        ++i;
    }
    return sum;
}
