/*
 * A396699  a(2) = 1; a(2*m) = m*(2*m^2+1)/3 - 1 for m >= 2, and a(2*m+1) =
 *          m*(m+1)*(2*m+1)/3 for m >= 1.
 * https://oeis.org/A396699
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2) = 1; a(2*m) = (m * (2*m*m + 1)) / 3 - 1 for m >= 2;
 *            a(2*m+1) = (m * (m + 1) * (2*m + 1)) / 3 for m >= 1
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;           /* values below the offset are irrelevant */
    if (n == 2)
        return (seq_t)1;           /* a(2) = 1 */

    if ((n & 1U) == 0) {           /* even n = 2*m, m >= 2 */
        const idx_t m = n >> 1;
        const seq_t sm  = (seq_t)m;
        const seq_t t2  = ((seq_t)2 * sm * sm) + (seq_t)1;
        return (sm * t2) / (seq_t)3 - (seq_t)1;
    } else {                       /* odd n = 2*m+1, m >= 1 */
        const idx_t m = n >> 1;
        const seq_t sm  = (seq_t)m;
        const seq_t t1  = sm + (seq_t)1;
        const seq_t t2  = ((seq_t)2 * sm) + (seq_t)1;
        return (sm * t1 * t2) / (seq_t)3;
    }
}
