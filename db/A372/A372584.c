/*
 * A372584  a(n) = number of odd digits in odd positions in the decimal
 *          expansion of n, where position 1 is the least significant digit.
 * https://oeis.org/A372584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of odd digits in odd positions of the decimal
 *            representation of n (position 1 is the least significant digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;          /* result fits in seq_t comfortably */
    idx_t x = n;              /* working copy of the index */
    unsigned int pos = 1;     /* position counter (1‑based) */

    /* Handle n = 0 specially: the loop below would skip, giving count 0 which is correct */
    while (x > 0) {
        unsigned int digit = x % 10;   /* extract least significant decimal digit */
        if ((pos & 1) && (digit & 1))  /* odd position and odd digit */
            ++count;
        x /= 10;                       /* shift right one decimal digit */
        ++pos;                         /* move to next position */
    }

    return count;
}
