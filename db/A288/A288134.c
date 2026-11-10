/*
 * A288134  Positions of 1 in A288132; complement of A288133.
 * https://oeis.org/A288134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(log2(n)) + 2 (complement of A288133: 1, 2,
 *            2^(k-2)+k-1 for k>=2)
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8429 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned lg = 0;
    idx_t m = n;
    while (m > 1) {
        m >>= 1;
        lg++;
    }
    return (seq_t)n + (seq_t)(lg + 2);
}
