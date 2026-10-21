/*
 * A253826  Indices of centered octagonal numbers (A016754) which are also
 *          triangular numbers (A000217).
 * https://oeis.org/A253826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) // Initial values: a(1)=1,
 *            a(2)=18, a(3)=595
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)595;

    seq_t prev3 = (seq_t)1;     /* a(n-3)  */
    seq_t prev2 = (seq_t)18;    /* a(n-2)  */
    seq_t prev1 = (seq_t)595;   /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = (seq_t)35 * prev1 - (seq_t)35 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
