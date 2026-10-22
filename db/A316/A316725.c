/*
 * A316725  Generalized 27-gonal (or icosiheptagonal) numbers: m*(25*m - 23)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A316725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = k * (25k + 23) / 2; a(2k+1) = (k+1) * (25k + 2) / 2,
 *            where k = floor(n/2)
 * Range:     u8 n <= 9, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        /* even term: n = 2k */
        seq_t k = nn >> 1;
        unsigned long long term = (unsigned long long)25 * (unsigned long long)k + 23ULL;
        unsigned long long prod = (unsigned long long)k * term;
        return (seq_t)(prod >> 1);            /* divide by 2 */
    } else {
        /* odd term: n = 2k + 1 */
        seq_t k = (nn - 1) >> 1;
        unsigned long long term = (unsigned long long)25 * (unsigned long long)k + 2ULL;
        unsigned long long kk   = (unsigned long long)k + 1ULL;
        unsigned long long prod = kk * term;
        return (seq_t)(prod >> 1);            /* divide by 2 */
    }
}
