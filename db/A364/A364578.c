/*
 * A364578  a(n) is the smallest n-digit number whose sum of digits is n.
 * https://oeis.org/A364578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the smallest n‑digit number whose decimal digit sum
 *            equals n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm constructs the digits from left to right: set the first digit
   to 1, then greedily place the remaining sum on the rightmost positions
   so that all earlier digits are as small as possible.  This yields the
   lexicographically smallest number while preserving the required sum. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0) return 0;               /* n is defined ≥1, but guard anyway */

    idx_t remSum = n - 1;               /* digits after the leading 1 */
    idx_t remPos = n - 1;               /* positions remaining after the first digit */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;

        /* leading digit is always 1 */
        if (i == 1) {
            digit = 1;
        } else {
            /* minimal digit that still allows the remaining sum to be
               achieved with the remaining positions. */
            if (remSum > 9 * (remPos - 1)) {
                digit = remSum - 9 * (remPos - 1);
            } else {
                digit = 0;
            }
        }

        result = result * 10 + digit;

        if (i > 1) {
            if (digit > 0) remSum -= (idx_t)digit;
            if (remPos > 0) remPos--;   /* one less position left */
        }
    }

    return result;
}
