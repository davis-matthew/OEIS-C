/*
 * A327952  a(n) is the number of positive integers k such that some multiple
 *          of sqrt(k) falls strictly between n and n+1.
 * https://oeis.org/A327952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{m=1}^{n+1} max(0, ceil((n+1)² / m²) – 1 – floor(n² /
 *            m²))
 * Range:     u8 n <= 14, u16 n <= 225, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the distinct positive integers k for which some multiple of √k
   lies strictly between n and n+1. */
seq_t a(idx_t n) {
    seq_t result = 0;
    /* Compute squares once to avoid repeated multiplication. */
    seq_t n_sq   = (seq_t)n * ((seq_t)n);
    seq_t n1 = (seq_t)n + 1;
    seq_t np1_sq = n1 * n1;

    for (idx_t m = 1; m <= (idx_t)(n + 1); ++m) {
        seq_t m_sq     = (seq_t)m * ((seq_t)m);
        seq_t floor_low  = n_sq / m_sq;                 /* floor(n² / m²) */
        seq_t ceil_high  = (np1_sq + m_sq - 1) / m_sq;   /* ceil((n+1)² / m²) */
        if (ceil_high > floor_low + 1) {
            result += (ceil_high - 1 - floor_low);
        }
    }
    return result;
}
