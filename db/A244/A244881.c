/*
 * A244881  Expansion of (1 + 26*x + 109*x^2 + 109*x^3 + 26*x^4 + x^5) / (1 -
 *          x)^8.
 * https://oeis.org/A244881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 34, u64 n <= 856, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A244881
 *  Expansion of (1 + 26*x + 109*x^2 + 109*x^3 + 26*x^4 + x^5) / (1 - x)^8
 *
 *  Formula: a(n) = (2520 + 11526*n + 22617*n^2 + 24724*n^3 + 16275*n^4 + 6454*n^5 + 1428*n^6 + 136*n^7) / 2520
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid intermediate overflow for n up to about 10 000. */
    unsigned __int128 y = 136;                      /* coefficient of n^7 */
    y = y * (unsigned __int128)n + 1428;            /* +1428*n^6 */
    y = y * (unsigned __int128)n + 6454;            /* +6454*n^5 */
    y = y * (unsigned __int128)n + 16275;           /* +16275*n^4 */
    y = y * (unsigned __int128)n + 24724;           /* +24724*n^3 */
    y = y * (unsigned __int128)n + 22617;           /* +22617*n^2 */
    y = y * (unsigned __int128)n + 11526;           /* +11526*n */
    y = y * (unsigned __int128)n + 2520;            /* +2520   (constant term) */
    y /= 2520;                                      /* final division, value is integral */
    return (seq_t)y;
}
