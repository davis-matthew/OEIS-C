/*
 * A261149  a(n) = 515486946529943 + (n-1)*30526020494970.
 * https://oeis.org/A261149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 515486946529943 + (n-1)*30526020494970
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)515486946529943ULL;
    seq_t diff = (seq_t)30526020494970ULL;

    if (n == 0)
        return base;               /* undefined for n < 1, return first term */

    seq_t nm1 = (seq_t)(n - 1);
    return base + nm1 * diff;
}
