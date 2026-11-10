/*
 * A327954  First differences of A327953.
 * https://oeis.org/A327954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A327953(n+1) - A327953(n), where A327953(n) =
 *            Sum_{m=2..n+1} ( floor(((n+1)^2-1)/m^2) - floor(n^2/m^2) )
 * Range:     i8 n <= 9, i16 n <= 81, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9900 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A327953_a(idx_t n)
{
    seq_t n_seq  = (seq_t)n;
    seq_t np1    = n_seq + (seq_t)1;
    seq_t n_sq   = n_seq * n_seq;
    seq_t np1_sq = np1 * np1;
    seq_t upper_limit = (np1_sq > (seq_t)0) ? np1_sq - (seq_t)1 : (seq_t)0;
    seq_t result = (seq_t)0;

    for (idx_t m = (idx_t)2; (seq_t)m <= np1; ++m)
    {
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t diff = upper_limit / m_sq - n_sq / m_sq;
        if (diff > (seq_t)0)
            result += diff;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A327953_a(n + 1) - A327953_a(n);
}
