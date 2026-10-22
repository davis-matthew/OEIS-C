/*
 * A371535  In the decimal representation of n, retain the k-th decimal digit
 *          if the k-th binary digit in n is 1.
 * https://oeis.org/A371535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = retain the k‑th decimal digit of n iff the k‑th most
 *            significant binary digit of n is 1, where k is the number of
 *            decimal digits of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count decimal digits and store them (reversed order). */
    unsigned int dec_len = 0;
    idx_t tmp = n;
    unsigned char digits[20];          /* enough for 64‑bit integers */
    do {
        digits[dec_len++] = tmp % 10;
        tmp /= 10;
    } while (tmp > 0);

    /* Count binary bit length. */
    unsigned int bitlen = 0;
    tmp = n;
    do {
        ++bitlen;
        tmp >>= 1;
    } while (tmp > 0);

    /* Build the result by keeping digits whose corresponding
       binary bit is 1. */
    seq_t result = 0;
    for (unsigned int i = 0; i < dec_len; ++i) {
        unsigned int digit  = digits[dec_len - 1 - i];  /* most significant first */
        unsigned int bitpos = bitlen - 1 - i;
        if ((n >> bitpos) & 1U)
            result = result * 10 + (seq_t)digit;
    }
    return result;
}
