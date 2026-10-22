/*
 * A320431  The number of tiles inside a regular n-gon created by lines that
 *          run from each of the vertices of the n edges orthogonal to these
 *          edges.
 * https://oeis.org/A320431
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A320431 a(3) = a(4) = 1 a(2n) = n^2 + (n-1)^2 for n ≥ 3 (i.e.
 *            for indices 6,8,10,…) a(2n+1) = 8n^2 – 1 for n ≥ 2 (i.e. for
 *            indices 5,7,9,…) Equivalent piecewise definition for n ≥ 3
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3u) return (seq_t)0;          /* values below the offset are undefined */
    if (n == 3u || n == 4u) return (seq_t)1;

    if ((n & 1u) == 0) {                  /* even index 2n, n ≥ 3 */
        idx_t k = n / 2u;
        seq_t kk = (seq_t)k;
        return kk * kk + (kk - 1) * (kk - 1);
    } else {                              /* odd index 2n+1, n ≥ 2 */
        idx_t k = (n - 1) / 2u;
        seq_t kk = (seq_t)k;
        return 8 * kk * kk - 1;
    }
}
