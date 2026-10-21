/*
 * A246645  Expansion of 1/(1 - 22*x + 81*x^2), used in A246643.
 * https://oeis.org/A246645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 81*a(n-2), a(0)=1, a(1)=22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;

    /* Iterate the linear recurrence; no dynamic allocation is required. */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)22;  /* a(n-1) */
    seq_t cur   = 0;          /* current value a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur   = ((seq_t)22 * prev1) - ((seq_t)81 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
