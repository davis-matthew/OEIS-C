/*
 * A332143  a(n) = 4*(10^(2*n+1)-1)/9 - 10^n.
 * https://oeis.org/A332143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) with initial
 *            values a(0)=3, a(1)=434, a(2)=44344
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases explicitly */
    if (n == 0) return 3;
    if (n == 1) return 434;
    if (n == 2) return 44344;

    /* Iteratively compute using the recurrence relation.
       The intermediate products stay within the limits of
       seq_t for all n whose result still fits in seq_t
       (in particular for 64‑bit builds up to n = 9). */
    seq_t prev3 = 3;          /* a(n-3) */
    seq_t prev2 = 434;        /* a(n-2) */
    seq_t prev1 = 44344;      /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 111 * prev1 - 1110 * prev2 + 1000 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
