/*
 * A267876  Binary representation of the n-th iteration of the "Rule 233"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1)-100*a(n-2) for n > 5. Initial terms: a(0) = 1
 *            a(1) = 0 a(2) = 10101 a(3) = 1110111 a(4) = 111111111 a(5) =
 *            11111111111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[] = {
        (seq_t)1,
        (seq_t)0,
        (seq_t)10101,
        (seq_t)1110111,
        (seq_t)111111111,
        (seq_t)11111111111
    };
    if (n < 6)
        return init[n];

    seq_t prev2 = init[4];   /* a(4) */
    seq_t prev1 = init[5];   /* a(5) */
    seq_t cur;
    for (idx_t i = 6; i <= n; ++i) {
        cur = (seq_t)101 * prev1 - (seq_t)100 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
