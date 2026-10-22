/*
 * A330298  a(n) is the number of subsets of {1..n} that contain exactly 1 odd
 *          and 2 even numbers.
 * https://oeis.org/A330298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2) * binomial(floor(n/2), 2)
 * Range:     u8 n <= 17, u16 n <= 102, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t odds  = (seq_t)((n + 1) / 2);   /* ceil(n/2)  */
    seq_t evens = (seq_t)(n / 2);         /* floor(n/2) */

    /* When evens < 2, binomial(evens,2) is zero */
    if (evens < 2)
        return (seq_t)0;

    /* comb = evens * (evens - 1) / 2  */
    seq_t comb = evens * (evens - 1) / (seq_t)2;

    return odds * comb;
}
