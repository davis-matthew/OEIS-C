/*
 * A265027  First differences of A048701 divided by 6.
 * https://oeis.org/A265027
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A048701(n) - A048701(n-1)) / 6 for n >= 2, where
 *            A048701(n) = n*2^L + reverse_bits(n), L = bit length of n
 * Range:     u8 n <= 15, u16 n <= 1023, u32 n <= 32767, u64 n <= 32767,
 *            u128 n <= 32767, big any n
 * Validated: standard, pass, 107 terms through n = 9224 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A048701_a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    unsigned m = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++m;
    }
    unsigned len = m + 1;
    seq_t rev = 0;
    for (unsigned i = 0; i <= m; ++i) {
        if ((n >> i) & (idx_t)1) {
            rev |= ((seq_t)1) << (len - 1 - i);
        }
    }
    return ((seq_t)n << len) + rev;
}

seq_t a(idx_t n) {
    seq_t cur = A048701_a(n);
    seq_t prev = A048701_a(n - 1);
    return (cur - prev) / (seq_t)6;
}
