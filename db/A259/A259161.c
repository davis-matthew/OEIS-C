/*
 * A259161  Positive pentagonal numbers (A000326) that are triangular numbers
 *          (A000217) divided by 2.
 * https://oeis.org/A259161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5, a(2)=48510, a(3)=465793515
   for n>=4  a(n) = 9603*a(n-1) - 9603*a(n-2) + a(n-3) */
seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)48510;
    if (n == 3) return (seq_t)465793515;

    /* iterative computation of the linear recurrence */
    seq_t prev3 = (seq_t)5;               /* a(n-3) */
    seq_t prev2 = (seq_t)48510;           /* a(n-2) */
    seq_t prev1 = (seq_t)465793515;       /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 9603*a(i-1) - 9603*a(i-2) + a(i-3) */
        next = (seq_t)9603 * prev1 - (seq_t)9603 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
