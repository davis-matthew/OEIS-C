/*
 * A350141  Times when clock hands are most nearly equidistant - expressed
 *          exactly as a(n)*43200/1427 seconds after 00:00:00.
 * https://oeis.org/A350141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = m + ((m - k) mod 2), where k = n + floor((n-1)/2) and m =
 *            floor(k*1427/33)
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nseq = (seq_t)n;
    seq_t k = nseq + (nseq - 1) / 2;
    seq_t m = (k * 1427) / 33;
    return m + ((m - k) & 1);
}
