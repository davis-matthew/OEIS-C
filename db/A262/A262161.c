/*
 * A262161  a(n) is the largest term in the continued fraction for a(n-1) +
 *          n^2/a(n-1), where a(1)=1.
 * https://oeis.org/A262161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) is the largest term in the continued
 *            fraction of a(n‑1) + n²/a(n‑1)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, but safe */
    seq_t prev = 1;                /* a(1) */
    if (n == 1) return prev;
    for (idx_t i = 2; i <= n; ++i) {
        /* rational number (prev² + i²)/prev */
        seq_t num = prev * prev + (seq_t)i * (seq_t)i;
        seq_t den = prev;
        seq_t maxterm = 0;
        while (den != 0) {
            seq_t term = num / den;
            if (term > maxterm) maxterm = term;
            seq_t rem = num % den;
            num = den;
            den = rem;
        }
        prev = maxterm;
    }
    return prev;
}
