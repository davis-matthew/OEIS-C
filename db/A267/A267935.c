/*
 * A267935  Decimal representation of the n-th iteration of the "Rule 249"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=27, a(3)=127, a(4)=511, for n>4
 *            a(n)=5*a(n-1)-4*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    if (n == 2) return 27;
    if (n == 3) return 127;
    if (n == 4) return 511;

    seq_t prev2 = 127; /* a(3) */
    seq_t prev1 = 511; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t diff = prev1 - prev2;         /* a(n-1) - a(n-2) */
        seq_t cur  = (diff << 2) + prev1;   /* 4*(a(n-1)-a(n-2)) + a(n-1) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
