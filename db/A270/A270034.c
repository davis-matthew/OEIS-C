/*
 * A270034  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 8 (or 0 if no such base exists).
 * https://oeis.org/A270034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer base b >= 9 such that the base‑b *
 *            representation of n contains the digit 8; if no such base exists
 *            * the function returns 0
 * Range:     u8 n <= 541, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special handling for n = 8 where base 9 works immediately */
    if (n == 8) {
        return (seq_t)9;
    }

    /* For 1 <= n <= 7, no base can contain 8 (all bases >= 9 produce a
     * single digit less than 8).  For n = 9, all bases >= 9 produce the
     * single digit 9, so return 0.  These cases are handled together. */
    if (n < 9) {
        return (seq_t)0;
    }

    /* General case: search from base 9 up to base n.  Any base larger
     * than n would represent n as a single digit n, which is never 8
     * for n >= 9. */
    seq_t n_val = (seq_t)n;          /* copy n into seq_t for arithmetic  */
    for (idx_t b = 9; b <= n; ++b) {
        seq_t base  = (seq_t)b;
        seq_t x     = n_val;

        /* Examine each digit of n in base 'base' */
        while (x > (seq_t)0) {
            seq_t digit = x % base;
            if (digit == (seq_t)8) {
                return (seq_t)b;          /* smallest base found */
            }
            x /= base;
        }
    }

    /* If the loop ends, no base contains digit 8 */
    return (seq_t)0;
}
