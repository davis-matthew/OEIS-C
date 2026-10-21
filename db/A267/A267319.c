/*
 * A267319  Continued fraction expansion of phi^8, where phi = (1 +
 *          sqrt(5))/2.
 * https://oeis.org/A267319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 46, a(n) = 23 + 22*(-1)^n for n > 0
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 46;                /* a(0) = 46 */
    if ((n & 1) == 0)            /* even n > 0  -> 23 + 22 = 45 */
        return 45;
    return 1;                    /* odd n   -> 23 - 22 = 1 */
}
