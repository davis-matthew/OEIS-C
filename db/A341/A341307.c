/*
 * A341307  Expansion of (x^9+x^8+2*x^7+x^6+2*x^5+2*x^4+x^3+x^2+1)/(1-x^6)^2.
 * https://oeis.org/A341307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2+(-1)^n)/3)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow when computing the numerator. */
    if ((n & 1ULL) == 0) {
        /* even n: (n+3)/3 */
        unsigned __int128 num = (unsigned __int128)n + 3;
        return (seq_t)(num / 3);
    } else {
        /* odd n: (n+1)/3 */
        unsigned __int128 num = (unsigned __int128)n + 1;
        return (seq_t)(num / 3);
    }
}
