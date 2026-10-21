/*
 * A270035  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 9 (or 0 if no such base exists).
 * https://oeis.org/A270035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b (>1) such that the base‑b representation
 *            of n contains the digit 9; 0 if no such base exists
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 9 yields base 10 (single digit 9). */
    if (n == 9)
        return (seq_t)10;

    /* For n < 9 or n = 10..18, a(n) = 0 (no base produces a 9). */
    if (n < 9)
        return (seq_t)0;

    /* Search bases from 2 up to n-1; bases > n produce a single digit
       equal to n, which cannot be 9 when n > 9. */
    for (idx_t b = 2; b < n; ++b) {
        seq_t base = (seq_t)b;
        seq_t temp = (seq_t)n;

        while (temp >= base) {
            seq_t digit = temp % base;
            if (digit == (seq_t)9)
                return base;
            temp /= base;
        }
        /* Check the most significant digit (temp < base). */
        if (temp == (seq_t)9)
            return base;
    }

    return (seq_t)0;
}
