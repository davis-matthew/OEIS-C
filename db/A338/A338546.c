/*
 * A338546  For n > 0, a(n) is the number of 1's among the first T(n) terms of
 *          the sequence 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, ..., k 1's, k
 *          0's, where T(n) is the n-th triangular number.
 * https://oeis.org/A338546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1's in the first n(n+1)/2 terms of the *
 *            run-length sequence that consists of k ones followed by k zeros,
 *            * for k = 1, 2, 3, ... . * Sequence:
 *            1,0,11,00,111,000,1111,0000,... * a(1) = 1, a(2) = 2, a(3) = 3,
 *            a(4) = 6,
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* T(n) = n(n+1)/2 */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* find the largest m such that m(m+1) <= t */
    idx_t low  = 0;
    idx_t high = n;   /* for all n >= 0, high*(high+1) > t except n=0 */

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t val = (seq_t)mid * ((seq_t)mid + 1);
        if (val <= t)
            low = mid;
        else
            high = mid;
    }

    idx_t m = low;
    seq_t m_seq = (seq_t)m;

    /* compute r = t - m(m+1) */
    seq_t sub   = m_seq * (m_seq + 1);
    seq_t r     = t - sub;

    /* ones before the current run: m(m+1)/2 */
    seq_t ones_before = sub / 2;

    /* add ones from the partial run */
    seq_t add = (r <= (m_seq + 1)) ? r : (m_seq + 1);

    return ones_before + add;
}
