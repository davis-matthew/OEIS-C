/*
 * A257287  a(n) = 6*7^n - 5*6^n.
 * https://oeis.org/A257287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*7^n - 5*6^n Recurrence: a(0)=1, a(1)=12;
 *            a(n)=13*a(n-1)-42*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 12;

    seq_t prev = 1;  /* a(n-2) */
    seq_t curr = 12; /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 13 * curr - 42 * prev;
        prev = curr;
        curr = next;
    }

    return curr;
}
