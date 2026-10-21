/*
 * A277131  Magic numbers of anti-Mackay icosahedra.
 * https://oeis.org/A277131
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10/3*n³ + 25*n² + 161/3*n + 45 with offset 0 (OEIS uses
 *            index starting at n = 2)
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 809, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)                /* no value defined below the offset */
        return 0;

    seq_t m = (seq_t)(n - 2); /* shift index so that m = 0 gives the first term */
    /* Compute the numerator: 10*m³ + 75*m² + 161*m + 135  (common denominator 3) */
    seq_t num = 10 * m * m * m + 75 * m * m + 161 * m + 135;
    return num / 3;           /* exact division */
}
