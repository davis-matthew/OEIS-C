/*
 * A327953  a(n) is the number of positive integers k such that some
 *          nontrivial multiple of sqrt(k) falls strictly between n and n+1.
 * https://oeis.org/A327953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=2}^{n+1} ( floor(((n+1)^2-1)/m^2) - floor(n^2/m^2) )
 *            This counts, for each integer m>1, the integers k for which
 *            m*sqrt(k) lies strictly between n and n+1. Each such k has a
 *            única multiplicador m, so the sums over m are disjoint
 * Range:     u8 n <= 14, u16 n <= 93, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t for computations */
    seq_t n_seq  = (seq_t)n;
    seq_t np1    = n_seq + (seq_t)1;
    seq_t n_sq   = n_seq * n_seq;
    seq_t np1_sq = np1 * np1;
    seq_t upper_limit = (np1_sq > (seq_t)0) ? np1_sq - (seq_t)1 : (seq_t)0;

    seq_t result = (seq_t)0;

    /* iterate over all possible non‑trivial multipliers m≥2 */
    for (idx_t m = (idx_t)2; m <= (idx_t)np1; ++m)
    {
        seq_t m_sq        = (seq_t)m * (seq_t)m;
        seq_t upper_floor = upper_limit / m_sq;
        seq_t lower_floor = n_sq / m_sq;
        seq_t diff = upper_floor - lower_floor;
        if (diff > (seq_t)0)
            result += diff;
    }

    return result;
}
