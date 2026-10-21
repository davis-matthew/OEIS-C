/*
 * A256506  a(n) = (2*n+3)*a(n-1) + a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A256506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n) = (2*n+3)*a(n-1) + a(n-2) for n >= 2 */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iterate iteratively; no memoisation required but minimal memory used */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute coefficient (2*i + 3) */
        seq_t coeff = ((seq_t)2 * (seq_t)i) + (seq_t)3;
        current = coeff * prev1 + prev2;  /* a(i) = coeff*a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;  /* a(n) */
}
