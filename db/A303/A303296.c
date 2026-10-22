/*
 * A303296  Digital roots of fourth powers A000583.
 * https://oeis.org/A303296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of n^4 = 1 + ((n^4 - 1) mod 9), which equals
 *            n^4 mod 9 with 0 mapped to 9
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but harmless */
    idx_t r = n % 9;                /* 0..8 */
    idx_t r2 = r * r;               /* 0..64 */
    idx_t r4 = (r2 * r2) % 9;       /* 0..8, digital root component */
    seq_t res = (r4 == 0) ? 9 : (seq_t)r4;
    return res;
}
