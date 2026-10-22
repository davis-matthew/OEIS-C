/*
 * A337085  Square array T(n, k) read by descending antidiagonals. T(n, k) =
 *          n! * Sum_{j=0..n} j^k / j!.
 * https://oeis.org/A337085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the square array T(n, k) of A337085 along
 *            descending antidiagonals. Let s be the unique integer satisfying
 *            s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 and let offset = n – s*(s+1)/2.
 *            Then a(n) = T(offset, s – offset), where T(0, k) = (k == 0 ? 1 :
 *            0) T(n, k) = n^k + n * T(n‑1, k) for n > 0. The...
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find s with s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* offset within the antidiagonal */
    idx_t offset = n - (s * (s + 1)) / 2;
    seq_t nn = (seq_t)offset;          /* row index   */
    seq_t kk = (seq_t)(s - offset);    /* column index */

    /* Handle the base case n = 0 */
    if (nn == 0) {
        return (kk == 0 ? (seq_t)1 : (seq_t)0);
    }

    /* Start with T(0, kk) */
    seq_t T = (kk == 0 ? (seq_t)1 : (seq_t)0);

    /* Apply the recurrence T(n, k) = n^k + n * T(n‑1, k) */
    for (idx_t i = 1; i <= nn; ++i) {
        seq_t pow = (seq_t)1;
        for (idx_t j = 0; j < kk; ++j) {
            pow *= (seq_t)i;
        }
        T = pow + (seq_t)i * T;
    }
    return T;
}
