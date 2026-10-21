/*
 * A266872  Binary representation of the n-th iteration of the "Rule 77"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10001*a(n-2) - 10000*a(n-4) for n > 3; initial values:
 *            a(0) = 1, a(1) = 10, a(2) = 10101, a(3) = 101010
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)10101;
    if (n == 3) return (seq_t)101010;

    /* a small rolling array of the last four values */
    seq_t t[4] = {(seq_t)1, (seq_t)10, (seq_t)10101, (seq_t)101010};

    /* iterate from 4 up to n, applying the recurrence */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t val = (seq_t)10001 * t[(i - 2) & 3] - (seq_t)10000 * t[(i - 4) & 3];
        t[i & 3] = val;
    }
    return t[n & 3];
}
