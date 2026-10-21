/*
 * A246715  n * Lucas(n) - (n - 1) * Lucas(n - 1).
 * https://oeis.org/A246715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*Lucas(n) - (n-1)*Lucas(n-1), n >= 1 * Lucas numbers:
 *            L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n is within the defined offset */
    if (n == 0)
        return (seq_t)0;            /* undefined for OEIS offset */
    if (n == 1)
        return (seq_t)1;            /* a(1) = 1 */

    /* Compute Lucas(n) and Lucas(n-1) iteratively */
    seq_t L_pp = (seq_t)2;   /* L(0) */
    seq_t L_p  = (seq_t)1;   /* L(1) */
    seq_t L_n  = (seq_t)0;   /* placeholder for current L(i) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        L_n  = L_p + L_pp;   /* L(i) */
        L_pp = L_p;          /* shift for next iteration */
        L_p  = L_n;
    }

    /* After loop: L_p = L(n), L_pp = L(n-1) */
    seq_t Ln   = L_p;
    seq_t Ln_1 = L_pp;

    /* Compute the sequence value using the formula */
    seq_t result = (seq_t)n * Ln - (seq_t)(n - 1) * Ln_1;
    return result;
}
