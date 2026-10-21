/*
 * A291520  The arithmetic function uhat(n,4,2).
 * https://oeis.org/A291520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 5; a(n) = a(n-2) for n > 5 Thus for n >=
 *            6: a(n) = 4 if n is even, 5 if n is odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1..5 are just n itself */
    if (n <= 5)
        return (seq_t)n;

    /* For n > 5 the sequence alternates 4,5 with period 2 */
    return (seq_t)((n & 1) ? 5 : 4);
}
