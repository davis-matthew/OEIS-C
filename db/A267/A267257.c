/*
 * A267257  Binary representation of the middle column of the "Rule 111"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies (1-x^2)(1-10x)G(x)=1+x-x^8. * Recurrence: a(0)=1,
 *            a(1)=11, a(2)=111; * for n≥3: a(n) = 10*a(n-1) + a(n-2) -
 *            10*a(n-3) + N(n) * where N(0)=1, N(1)=1, N(8) = -1, and N(n)=0
 *            otherwise (n>1, n≠8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 11;
    if (n == 2) return 111;

    seq_t a0 = 1;   /* a(n-3)   */
    seq_t a1 = 11;  /* a(n-2)   */
    seq_t a2 = 111; /* a(n-1)   */

    for (idx_t k = 3; k <= n; ++k) {
        seq_t tmp = a2 * 10 + a1 - a0 * 10; /* 10*a(n-1)+a(n-2)-10*a(n-3)   */
        if (k == 8)               /* N(8) = -1                       */
            tmp -= 1;
        a0 = a1;
        a1 = a2;
        a2 = tmp;
    }

    return a2;
}
