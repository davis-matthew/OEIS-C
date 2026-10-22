/*
 * A392521  Irregular table: apply Ducci map to each binary string of length
 *          n-1 in order and read as binary digits to get the terms of row n.
 * https://oeis.org/A392521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is obtained by dropping the leading 1 of the binary
 *            expansion of n, taking the remaining bits (call them x),
 *            right‑rotating x by 1 bit, and XORing x with that rotation
 * Range:     u8 n <= 512, u16 n <= 9913, u32 n <= 9913, u64 n <= 9913,
 *            u128 n <= 9913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Floor log₂ for unsigned 64‑bit values.  Used to find the number of bits
   after the leading 1 in n.  The function is trivial and portable. */
static unsigned floor_log2_uint64(idx_t v)
{
    unsigned r = 0;
    while (v > 1) {
        v >>= 1;
        r++;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* n must be ≥ 1 as per the OEIS offset. */
    if (n < 2)            /* n == 1: mask width = 0 → a(1) = 0 */
        return 0;

    /* Number of bits after the leading 1. */
    unsigned m = floor_log2_uint64(n);

    if (m == 0)           /* handles n = 1 case */
        return 0;

    /* Mask to extract the lower m bits of n. */
    seq_t mask = ((seq_t)1 << m) - (seq_t)1;

    seq_t bits = (seq_t)n & mask;            /* remaining bits of n */
    seq_t rot  = (bits >> 1) | ((bits & (seq_t)1) << (m - 1)); /* right rotate by 1 */
    return bits ^ rot;
}
