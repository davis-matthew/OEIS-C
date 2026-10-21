/*
 * A266365  Number of possible plugboard settings for a WWII German Enigma
 *          Cipher Machine with n cables.
 * https://oeis.org/A266365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26! / ((26 - 2n)! n! 2^n)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 13 the factorial denominator is undefined, so the value is 0. */
    if (n > 13ULL)
        return 0;

    seq_t result = 1;
    for (idx_t k = 0; k < n; ++k)
    {
        /* Numerators: (26 - 2k) * (26 - 2k - 1) */
        seq_t first  = 26 - 2 * k;
        seq_t second = 26 - 2 * k - 1;

        /* Denominator: 2 * (k + 1) = 2k + 2 */
        seq_t denom = 2 * (k + 1);

        /* Multiply and divide exactly; the division is guaranteed to be exact. */
        result = result * first * second / denom;
    }

    return result;
}
