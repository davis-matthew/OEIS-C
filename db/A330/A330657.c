/*
 * A330657  Number of ways the n-th pentagonal number A000326(n) can be
 *          written as the difference of two positive pentagonal numbers.
 * https://oeis.org/A330657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (k,m) with 1 ≤ k < m such that * (3m²−m)
 *            − (3k²−k) = (3n²−n), i.e. the nth pentagonal number can be *
 *            expressed as the difference of two positive pentagonal numbers
 * Range:     u8 n <= 10, u16 n <= 1228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations of the nth pentagonal number as the
 * difference of two larger pentagonal numbers.
 * Uses an O(n) algorithm derived from
 * 3(m²−k²) − (m−k) = 3n² − n  with d = m − k > 0. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;                /* safety for n<1 */
    /* Precompute constants */
    seq_t n_sq  = (seq_t)n * (seq_t)n;    /* n^2   */
    seq_t rhs   = 3 * n_sq - (seq_t)n;     /* 3n^2 – n */

    /* Iterate over possible differences d = m – k */
    for (idx_t d = 1; d < n; ++d) {
        seq_t d_sq = (seq_t)d * (seq_t)d;  /* d^2   */
        seq_t N    = rhs - 3 * d_sq + (seq_t)d;   /* numerator in k formula */
        if (N <= 0) continue;          /* k would be non‑positive  */
        seq_t denom = 6 * (seq_t)d;     /* 6d   */
        if (N % denom == 0) {
            /* k = N / denom > 0 automatically, so this yields a valid pair (k,m) */
            ++count;
        }
    }
    return count;
}
