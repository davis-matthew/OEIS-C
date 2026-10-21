/*
 * A241204  Expansion of (1 + 2*x)^2/(1 - 2*x)^2.
 * https://oeis.org/A241204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=8; a(2)=32; a(n)=4*a(n-1)-4*a(n-2) for n≥3 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 8;
    if (n == 2) return 32;

    seq_t prev2 = 8;   /* a(1) */
    seq_t prev1 = 32;  /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 * (seq_t)4 - prev2 * (seq_t)4;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
