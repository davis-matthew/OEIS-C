/*
 * A387659  a(n) = 13*n^2 + 10*n + 3.
 * https://oeis.org/A387659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n^2 + 10*n + 3
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;                    /* cast index to seq_t */
    seq_t term = (seq_t)13 * x * x; /* 13*n^2 */
    term += (seq_t)10 * x;          /* +10*n */
    term += 3;                      /* +3 */
    return term;
}
