/*
 * A384328  Expansion of 1 / ((1-x)^3 * (1-x^7)).
 * https://oeis.org/A384328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3) * (n^2 + 12*n + 26) / 42)
 * Range:     u8 n <= 10, u16 n <= 135, u32 n <= 1648, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t t1   = nn + (seq_t)3;
    seq_t t2   = nn * nn + (seq_t)12 * nn + (seq_t)26;
    return (t1 * t2) / (seq_t)42;
}
