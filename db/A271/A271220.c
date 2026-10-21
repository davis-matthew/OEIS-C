/*
 * A271220  Concatenate sum of digits of previous term and product of digits
 *          of previous term, starting with 6.
 * https://oeis.org/A271220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0)=6, a(1)=66, a(n)=concat_parts(digit_sum(a(n-1)),
 *            digit_product(a(n-1))) for n>=2
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This simplifies to a(n)=1236 for all n>=2. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)6;
    if (n == 1)
        return (seq_t)66;
    return (seq_t)1236;
}
