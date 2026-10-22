/*
 * A335979  Number of partitions of n into exactly two parts with no decimal
 *          carries.
 * https://oeis.org/A335979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceil((1/2) * ∏(digit_i + 1)) - 1 for decimal digits of n
 *            * (equivalently a(n) = ceiling((1/2) * A089898(n)) - 1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;                         /* product of (digit+1) */
    if (n == 0) {                           /* special case for n = 0 */
        prod = 1;                           /* digit 0 gives factor 1 */
    } else {
        idx_t t = n;
        while (t > 0) {
            unsigned digit = t % 10;        /* extract decimal digit */
            prod *= (seq_t)(digit + 1);     /* multiply factor for this digit */
            t /= 10;
        }
    }

    /* ceil(prod/2) */
    seq_t half = prod / 2;
    if (prod & 1) half += 1;

    /* subtract 1 as per definition */
    return half - 1;
}
