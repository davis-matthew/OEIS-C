/*
 * A289765  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 597", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   binary representation of a(n) consists of k = floor((n+1)/4)
 *            pairs of '10' * followed by a tail of ones: * if n % 4 <= 1 →
 *            tail length = 2k + 1 * if n % 4 == 2 → tail length = 2k + 2 * if
 *            n % 4 == 3 → tail length = 2k * (For n % 4 == 1, a(n) equals
 *            a(n−1).)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 4;
    idx_t k = n / 4;          /* number of '10' pairs, adjusted below for r == 3 */
    if (r == 3) ++k;          /* for n % 4 == 3, one more pair */
    idx_t tail_len;
    if (r == 0 || r == 1)
        tail_len = 2 * k + 1;
    else if (r == 2)
        tail_len = 2 * k + 2;
    else /* r == 3 */
        tail_len = 2 * k;

    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i) {
        val <<= 2;
        val |= (seq_t)2;      /* append '10' */
    }
    for (idx_t i = 0; i < tail_len; ++i) {
        val <<= 1;
        val |= (seq_t)1;      /* append '1' */
    }
    return val;
}
