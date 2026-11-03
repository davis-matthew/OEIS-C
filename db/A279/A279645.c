/*
 * A279645  a(n) = not (n XOR (n shift 1)).
 * https://oeis.org/A279645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = not (n XOR (n shift 1))
 * Range:     u8 n <= 469, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9696 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _bit_mask(unsigned bits)
{
    seq_t mask = 0;
    for (unsigned i = 0; i < bits; ++i) {
        mask = (mask << 1) | 1;
    }
    return mask;
}

seq_t a(idx_t n)
{
    /* Convert the index to seq_t so bitwise ops work on the correct width */
    seq_t sn = (seq_t)n;

    /* Find the number of bits needed to represent n */
    unsigned bits = 0;
    seq_t tmp = sn;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    /* Compute n XOR (n >> 1) and its bitwise complement */
    seq_t xored   = sn ^ (sn >> 1);
    seq_t inverted = ~xored;

    /* Mask out higher bits that are not part of n’s representation */
    seq_t mask = _bit_mask(bits);

    return inverted & mask;
}
