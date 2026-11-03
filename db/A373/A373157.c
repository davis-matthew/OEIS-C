/*
 * A373157  a(n) = 1 if the 2-adic valuation of n is a multiple of 3,
 *          otherwise 0.
 * https://oeis.org/A373157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if the 2-adic valuation of n is a multiple of 3, else 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9951 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return 0;          /* undefined for offset < 1, return 0 safely */
    }

    unsigned e = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) {
        ++e;
        m >>= 1ULL;
    }

    return (seq_t)((e % 3u) == 0u);
}
