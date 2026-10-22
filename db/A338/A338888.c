/*
 * A338888  a(n) = (a(n-2) bitwise-OR a(n-1)) + 1; a(1)=0, a(2)=0.
 * https://oeis.org/A338888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-2) | a(n-1)) + 1; a(1)=0, a(2)=0;
 * Range:     u8 n <= 25, u16 n <= 385, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n <= 1) return 0;            /* a(1) */
    if (n == 2) return 0;            /* a(2) */

    /* Iterate from term 3 up to n using only two previous values. */
    seq_t prev2 = 0;                 /* a(n-2) */
    seq_t prev1 = 0;                 /* a(n-1) */
    seq_t curr  = 0;                 /* current term a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        curr = (prev2 | prev1) + 1;  /* recurrence */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
