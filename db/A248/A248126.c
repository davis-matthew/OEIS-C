/*
 * A248126  a(n) = n^2 with each digit repeated.
 * https://oeis.org/A248126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n^2 with each decimal digit repeated twice
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 199, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0; return 0 just in case. */
    if (n == 0) return 0;

    /* Compute n^2.  Use seq_t for the intermediate product. */
    seq_t sq = (seq_t)n * (seq_t)n;

    seq_t result = 0;
    seq_t multiplier = 1;   /* 10^(2*position) starts with 10^0 = 1 */

    while (sq > 0) {
        seq_t d = sq % 10;           /* current decimal digit of n^2 */
        result += d * 11 * multiplier; /* dd = 11*d */
        multiplier *= 100;           /* move two decimal places for next digit */
        sq /= 10;
    }

    return result;
}
