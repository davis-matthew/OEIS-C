/*
 * A292371  A binary encoding of 1-digits in the base-4 representation of n.
 * https://oeis.org/A292371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary encoding of 1-digits in the base-4 representation
 *            of n
 * Range:     u8 n <= 8830, u16 n <= 8830, u32 n <= 8830, u64 n <= 8830,
 *            u128 n <= 8830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    int pos = 0;
    while (n > 0) {
        idx_t digit = n % 4;
        if (digit == 1)
            res |= ((seq_t)1 << pos);
        n /= 4;
        pos++;
    }
    return res;
}
