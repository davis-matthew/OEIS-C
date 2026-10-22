/*
 * A338761  Subword complexity of a the infinite word Product_{i>=1}
 *          Product_{j=1..i} a^j b^(i-j+1).
 * https://oeis.org/A338761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for n <= 3 * a(n) = (2*(n^3-4*n) + 3*(19+(-1)^n)) /
 *            12 for n >= 4 * The expression is an integer for all n; division
 *            is exact
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 1324, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n: 2^n */
    if (n <= 3)
        return (seq_t)1 << (unsigned)n;

    seq_t ns = (seq_t)n;

    /* Compute n^3 = n * n * n */
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;

    /* part1 = 2 * (n^3 - 4n) */
    seq_t temp   = n3 - (seq_t)4 * ns;   /* n^3 - 4n */
    seq_t part1  = temp * (seq_t)2;       /* 2*(n^3-4n) */

    /* part2 = 3 * (19 + (-1)^n) */
    /* (-1)^n : 1 if n even, -1 if n odd */
    /* 19 + (-1)^n gives 20 for even, 18 for odd */
    seq_t part2 = (seq_t)3 * (((n & 1U) != 0U) ? 18U : 20U);

    /* numerator = part1 + part2 */
    seq_t numerator = part1 + part2;

    /* a(n) = numerator / 12  (exact division) */
    return numerator / (seq_t)12;
}
