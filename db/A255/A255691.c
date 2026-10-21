/*
 * A255691  Convert n to base 6, move the most significant digit to the least
 *          significant one and convert back to base 10.
 * https://oeis.org/A255691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = move the most significant base‑6 digit of n to the least
 *            significant position, then interpret the resulting base‑6 number
 *            as base‑10
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* trivial case: single‑digit base‑6 numbers stay the same */
    if (n < 6)
        return (seq_t)n;

    /* find the largest power of 6 that divides n or is <= n */
    idx_t divisor = 1;
    while (divisor <= n / 6)
        divisor *= 6;

    /* msd = most significant digit, remainder = the remaining digits */
    idx_t msd      = n / divisor;
    idx_t remainder = n % divisor;

    /* rotate: append msd as the least significant digit */
    seq_t result = ((seq_t)remainder * 6) + (seq_t)msd;
    return result;
}
