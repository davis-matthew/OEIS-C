/*
 * A361976  (2,2)-block array, B(2,2), of the natural number array (A000027),
 *          read by descending antidiagonals.
 * https://oeis.org/A361976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = 8*s^2 - 20*s + 8*i + 11 * where s = smallest integer
 *            with s*(s-1)/2 >= n (s ≥ 2), * i = n - (s-1)*(s-2)/2 (1 ≤ i ≤
 *            s-1). * Then j = s - i. * This uses b(i,j) = 8(i+j)^2 - 12i -
 *            20j + 11
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that s*(s-1)/2 >= n. */
    idx_t s = 2;
    idx_t cum = 1;                   /* Σ_{t=2}^{s} (t-1) = s(s-1)/2   */
    while (cum < n) {
        s++;
        cum += (s - 1);
    }

    /* Position along the diagonal (1‑based). */
    idx_t prev = cum - (s - 1);      /* Σ_{t=2}^{s-1} (t-1)              */
    idx_t i = n - prev;              /* 1 ≤ i ≤ s-1                     */

    /* Compute the value using the simplified formula. */
    seq_t ss = (seq_t)s;
    seq_t ii = (seq_t)i;
    return 8 * ss * ss - 20 * ss + 8 * ii + 11;
}
