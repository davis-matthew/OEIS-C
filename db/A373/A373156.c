/*
 * A373156  a(n) = 1 if the 2-adic and the 3-adic valuations of n are equal,
 *          otherwise 0.
 * https://oeis.org/A373156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [v2(n) == v3(n)]
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9705 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    unsigned e2 = 0, e3 = 0;

    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1ULL;
    }

    m = n;
    while (m % 3ULL == 0) {
        e3++;
        m /= 3ULL;
    }

    return (e2 == e3) ? (seq_t)1 : (seq_t)0;
}
