/*
 * A368361  Comma transform of the odd numbers.
 * https://oeis.org/A368361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*((2n+1) mod 10) + first_digit(2n+3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here first_digit(x) is the most significant decimal digit of x. */
seq_t a(idx_t n)
{
    /* the (n)-th odd number */
    idx_t m1 = 2 * n + 1;
    /* the next odd number */
    idx_t m2 = m1 + 2;

    /* last decimal digit of m1 */
    unsigned d1 = (unsigned)(m1 % 10);

    /* most significant decimal digit of m2 */
    unsigned d2 = (unsigned)m2;
    while (d2 >= 10)
        d2 /= 10;

    return (seq_t)(d1 * 10 + d2);
}
