/*
 * A329990  Beatty sequence for the number x satisfying 1/x + 1/3^x = 1.
 * https://oeis.org/A329990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * x) where x = 1.310569941539718... (root of 1/x
 *            + 1/3^x = 1)
 * Range:     u8 none, u16 n <= 3, u32 n <= 28, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* rational approximation of x:
       x ≈ 1310569941539718 / 10^15  = 1.310569941539718  */
    static const seq_t P = 1310569941539718ULL;   /* numerator   */
    static const seq_t Q = 1000000000000000ULL;   /* 10^15  */

    /* multiply using unsigned __int128 to avoid overflow for n ≤ 10^4
       and cast the result back to seq_t (which can be 64‑bit or arbitrary). */
    return (seq_t)(((__int128)n * P) / Q);
}
