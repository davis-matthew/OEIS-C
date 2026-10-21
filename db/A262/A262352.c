/*
 * A262352  a(n) = Sum_{k=0..n} (-1)^k*floor(k^(1/4)).
 * https://oeis.org/A262352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * floor(k^(1/4))
 * Range:     i8 n <= 9669, i16 n <= 9669, i32 n <= 9669, i64 n <= 9669,
 *            i128 n <= 9669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* We group indices by their integer fourth-root.  For a given
       m >= 1, every k with m^4 <= k < (m+1)^4 has floor(k^(1/4)) == m.
       A block of such k contributes m * Σ_{k=start}^{end} (-1)^k,
       where start = m^4 and end = min((m+1)^4-1, n) */
    for (idx_t m = 1;; ++m)
    {
        /* Compute m^4 and (m+1)^4 using 128-bit arithmetic to avoid overflow */
        unsigned __int128 m4   = (unsigned __int128)m * m * m * m;
        if (m4 > n) break;                 /* no further contributions */
        unsigned __int128 m1_4 = (unsigned __int128)(m + 1);
        m1_4 = m1_4 * m1_4 * m1_4 * m1_4;

        idx_t start = (idx_t)m4;
        idx_t end   = (idx_t)((m1_4 - 1 > n) ? n : m1_4 - 1);

        /* Number of terms in this block */
        idx_t t = end - start + 1;
        if ((t & 1) == 0) continue;        /* even number of terms => cancellation */

        /* When t is odd the block sum is either +m or -m depending on
           the parity of the starting index. */
        seq_t sign = (start & 1) ? -1 : 1;
        sum += sign * (seq_t)m;
    }
    return sum;
}
