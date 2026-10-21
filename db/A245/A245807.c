/*
 * A245807  a(n) = 7^n + 10^n.
 * https://oeis.org/A245807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=17, a(n)=17*a(n-1)-70*a(n-2) */
/* This function computes a(n) = 7^n + 10^n for n >= 0.  */
seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 17;

    seq_t prev2 = 2;      /* a(0) */
    seq_t prev1 = 17;     /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)17 * prev1 - (seq_t)70 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
