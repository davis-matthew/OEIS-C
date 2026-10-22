/*
 * A385552  Period of {binomial(N,n) mod 5: N in Z}.
 * https://oeis.org/A385552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest power of 5 > n
 * Range:     u8 n <= 124, u16 n <= 625, u32 n <= 625, u64 n <= 625,
 *            u128 n <= 625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* Multiply by 5 until the value exceeds n */
    while (result <= (seq_t)n) {
        result *= 5;
    }
    return result;
}
