/*
 * A318556  a(n) is the number of lesser tetrahedral numbers that divide the
 *          n-th tetrahedral number.
 * https://oeis.org/A318556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n-1} [ T(n) mod T(m) == 0 ] where T(k) =
 *            k*(k+1)*(k+2)/6 is the k‑th tetrahedral number
 * Range:     u8 n <= 10, u16 n <= 73, u32 n <= 4381, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1U) {               /* no lesser tetrahedral numbers */
        return (seq_t)0;
    }

    /* Compute the n‑th tetrahedral number */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) * ((seq_t)n + 2) / 6;
    seq_t count = 0;

    for (idx_t m = 1U; m < n; ++m) {
        /* Compute the m‑th tetrahedral number */
        seq_t tm = (seq_t)m * ((seq_t)m + 1) * ((seq_t)m + 2) / 6;
        if (tn % tm == 0) {
            ++count;
        }
    }
    return count;
}
