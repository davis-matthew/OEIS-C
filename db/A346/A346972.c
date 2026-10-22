/*
 * A346972  a(1)=1, a(2)=4, a(n) = (n*a(n-1) - (n-1)*a(n-2)) mod (n+1).
 * https://oeis.org/A346972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n * a(n-1) - (n-1) * a(n-2)) mod (n+1) (n ≥ 3) with a(1)
 *            = 1, a(2) = 4
 * Range:     u8 n <= 20, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the initial two terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* iterative computation – O(n) time, no memoisation needed */
    seq_t prev2 = (seq_t)1;                     /* a(1) */
    seq_t prev1 = (seq_t)4;                     /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t mod   = (seq_t)(i + 1);           /* modulus (i+1) */
        seq_t term1 = (seq_t)i * prev1;         /* i * a(i-1) */
        seq_t term2 = (seq_t)(i - 1) * prev2;   /* (i-1) * a(i-2) */

        /* (term1 - term2) mod mod, ensuring non‑negative result */
        cur = ((term1 % mod) + mod - (term2 % mod)) % mod;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* a(n) */
}
