/*
 * A390418  Partial sums of A014829.
 * https://oeis.org/A390418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (72*6^n - 25*n^2 - 85*n - 72)/250
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 6^n in a 128‑bit integer to avoid overflow on 64‑bit builds */
    unsigned __int128 pow6 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow6 *= 6;

    /* Build the numerator in 128‑bit, then divide by 250 */
    unsigned __int128 num   = (unsigned __int128)72 * pow6;
    unsigned __int128 n128  = n;
    num -= (unsigned __int128)25 * n128 * n128;
    num -= (unsigned __int128)85 * n128;
    num -= 72;

    num /= 250;

    return (seq_t)num;
}
