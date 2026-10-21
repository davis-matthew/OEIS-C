/*
 * A284922  Numbers with digits 2 and 8 only.
 * https://oeis.org/A284922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * A032822(n) (numbers with digits 2 and 8 only)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the length L such that the nth word lies in the block of
       all binary words of length L (1-indexed).  We keep a running total
       of the number of words of all smaller lengths. */
    idx_t total = 0;          /* number of words of lengths < L */
    idx_t power = 2;          /* 2^L for current L (starting with L=1) */
    unsigned int length = 1;  /* current L */

    while (total + power < n) {
        total += power;
        power <<= 1;          /* next power of two */
        ++length;
    }

    /* Offset of the desired word inside the current length block:
       0-based index within the block of 2^L words. */
    idx_t offset = n - total - 1;

    /* Convert the binary word that represents 'offset' into the
       corresponding decimal number consisting of digits 2 and 8. */
    seq_t result = 0;
    for (unsigned int pos = 0; pos < length; ++pos) {
        /* Extract the bit at the (length-1-pos) position. */
        if ((offset >> (length - 1 - pos)) & 1)
            result = result * 10 + 8;
        else
            result = result * 10 + 2;
    }

    return result;
}
