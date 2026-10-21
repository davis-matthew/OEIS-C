/*
 * A291358  The arithmetic function u(n,2,4).
 * https://oeis.org/A291358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n even; a(n) = 3 if n odd and n mod 3 == 0;
 *            otherwise a(n) = 5
 * Range:     u8 n <= 8889, u16 n <= 8889, u32 n <= 8889, u64 n <= 8889,
 *            u128 n <= 8889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; test evenness first */
    if ((n & 1) == 0)
        return 2;

    /* n is odd; check divisibility by 3 */
    if (n % 3 == 0)
        return 3;

    return 5;
}
