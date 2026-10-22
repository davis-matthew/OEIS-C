/*
 * A309954  Product of digits of (n written in base 4).
 * https://oeis.org/A309954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the base‑4 digits of n (a(0)=0)
 * Range:     u8 n <= 2814, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* by definition a(0)=0 */

    seq_t prod = (seq_t)1;
    idx_t t = n;
    while (t > 0) {
        seq_t digit = (seq_t)(t % 4);
        prod *= digit;
        if (prod == 0)          /* a zero digit makes the product zero */
            return (seq_t)0;
        t /= 4;
    }
    return prod;
}
