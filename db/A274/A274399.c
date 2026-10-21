/*
 * A274399  a(n) = floor(n*Pi/3).
 * https://oeis.org/A274399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*Pi/3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Scaling factor 10^10 is sufficient for any n up to 10^10-1
       (and far beyond the limits used in the tests).  The integer
       value floor(Pi*10^10) equals 31415926535.  Dividing this by 3
       gives an integer 10471975511 which represents Pi/3 scaled by
       10^10.  The final division by 10^10 yields the desired floor. */
    static const seq_t K     = (seq_t)10471975511ULL; /* floor(Pi*10^10 / 3) */
    static const seq_t SCALE = (seq_t)10000000000ULL; /* 10^10        */
    return ((seq_t)n * K) / SCALE;
}
