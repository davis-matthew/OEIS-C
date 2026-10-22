/*
 * A364636  a(n) = ((1 - sqrt(2))^n + (1 + sqrt(2))^n)*n/2.
 * https://oeis.org/A364636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((1+√2)^n + (1-√2)^n)/2 Equivalent linear recurrence:
 *            L_0 = 2, L_1 = 2, L_n = 2*L_{n-1} + L_{n-2} (L_n = α^n + β^n)
 *            a(n) = n * L_n / 2 Computes a(n) for all n >= 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Compute L_n using the recurrence. */
    seq_t L_prevprev = (seq_t)2; /* L_0 */
    seq_t L_prev     = (seq_t)2; /* L_1 */
    seq_t L_cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* L_i = 2*L_{i-1} + L_{i-2} */
        L_cur = (L_prev + L_prev) + L_prevprev;
        L_prevprev = L_prev;
        L_prev = L_cur;
    }

    /* L_n is now in L_prev.  L_n is even for n ≥ 1. */
    seq_t half_Ln = L_prev / (seq_t)2;
    return (seq_t)n * half_Ln;
}
