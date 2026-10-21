/*
 * A265228  Interleave the even numbers with the numbers that are congruent to
 *          {1, 3, 7} mod 8.
 * https://oeis.org/A265228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for even n a(n) = n + 2*floor((n+1)/6) for odd n This
 *            implements OEIS sequence A265228
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1ULL) == 0)
        return (seq_t)n;

    /* odd indices: compute floor((n+1)/6) without overflow */
    idx_t q = n / 6;          /* integer quotient */
    idx_t r = n % 6;          /* remainder */
    seq_t add = (seq_t)q + ((r == 5) ? 1 : 0); /* floor((n+1)/6) */

    return (seq_t)n + (add << 1); /* add * 2 */
}
