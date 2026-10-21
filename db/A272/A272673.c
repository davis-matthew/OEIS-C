/*
 * A272673  Take list of squares that start with 1 (A045784) and omit the
 *          leading 1 and any leading zeros from what is left; if the number
 *          was a power of 10, replace it with 0.
 * https://oeis.org/A272673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (k^2 - 10^⌊log₁₀(k²)⌋) for the n‑th integer k whose
 *            square starts with the digit 1
 * Range:     u8 n <= 7, u16 n <= 151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety; OEIS offset is 1 */
    idx_t count = 0;
    idx_t m = 1;
    seq_t result = 0;
    seq_t p10 = 1;                            /* 10^d for the current square */

    while (count < n) {
        seq_t sq = (seq_t)m * (seq_t)m;       /* square of m */

        /* Advance p10 until 10^d <= sq < 10^(d+1) */
        while (sq >= p10 * 10) {
            p10 *= 10;
        }

        /* If leading digit is 1 (sq < 2*10^d), record the value */
        if (sq < p10 * 2) {
            result = sq - p10;
            count++;
        }

        m++;
    }
    return result;
}
