/*
 * A268411  Parity of number of runs of 1's in binary representation of n.
 * https://oeis.org/A268411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of the number of runs of 1's in the binary representation of n. */
seq_t a(idx_t n)
{
    unsigned int parity = 0;
    idx_t prev = 0;

    while (n)
    {
        idx_t bit = n & 1;

        if (bit && !prev)
            parity ^= 1;

        prev = bit;
        n >>= 1;
    }

    return (seq_t)parity;
}
