/*
 * A267274  Decimal representation of the n-th iteration of the "Rule 117"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-4)^n+(-1)^n+2^(2*n+1)-1)/2 for n>1. Recurrence: a(n) =
 *            17*a(n-2) - 16*a(n-4) for n>5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t a0 = (seq_t)1;
    static const seq_t a1 = (seq_t)3;
    static const seq_t a2 = (seq_t)24;
    static const seq_t a3 = (seq_t)31;
    static const seq_t a4 = (seq_t)384;
    static const seq_t a5 = (seq_t)511;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;

    /* Use the recurrence separately for even and odd indices. */
    if ((n & 1) == 0)          /* n is even */
    {
        /* a(0)=1, a(2)=24, a(4)=384 are known. */
        seq_t prev2 = a2;     /* a(n-4) */
        seq_t prev1 = a4;     /* a(n-2) */

        for (idx_t i = 6; i <= n; i += 2)
        {
            seq_t curr = 17 * prev1 - 16 * prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
    else                       /* n is odd */
    {
        /* a(1)=3, a(3)=31, a(5)=511 are known. */
        seq_t prev2 = a3;     /* a(n-4) */
        seq_t prev1 = a5;     /* a(n-2) */

        for (idx_t i = 7; i <= n; i += 2)
        {
            seq_t curr = 17 * prev1 - 16 * prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
}
