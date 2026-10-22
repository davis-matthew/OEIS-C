/*
 * A374243  a(n) = ceiling(1/2 (n - 3 - ceiling((n - 3)/3))*ceiling((n -
 *          3)/3)).
 * https://oeis.org/A374243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(1/2 * (n - 3 - ceil((n - 3)/3)) * ceil((n -
 *            3)/3))
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 4 the formula yields 0, but the unsigned arithmetic would
       underflow, so we return 0 explicitly. */
    if (n <= 4)
        return (seq_t)0;

    seq_t t = (seq_t)n - (seq_t)3;          // n - 3
    seq_t k = (t + (seq_t)2) / (seq_t)3;    // ceil((n-3)/3)

    seq_t prod = (t - k) * k;               // (n-3 - k) * k
    return (prod + (seq_t)1) / (seq_t)2;    // ceil(prod / 2)
}
