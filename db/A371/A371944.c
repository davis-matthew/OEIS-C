/*
 * A371944  The binary expansion of a(n) corresponds to the ordinal transform
 *          (reduced modulo 2) of the binary expansion of n.
 * https://oeis.org/A371944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A371944: a(n) is obtained by taking the binary
   expansion of n, computing the ordinal transform, reducing each
   ordinal modulo 2, and interpreting the resulting bits as a new
   binary number.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* by definition a(0)=0 */

    seq_t result = 0;
    unsigned int p0 = 0;    /* parity of counts of 0 */
    unsigned int p1 = 0;    /* parity of counts of 1 */

    /* find the position of the most‑significant 1 bit of n */
    idx_t msb = 0;
    for (idx_t i = sizeof(idx_t) * 8 - 1; i != (idx_t)(-1); --i) {
        if ((n >> i) != 0) {
            msb = i;
            break;
        }
    }

    /* iterate from most significant to least significant bit */
    for (idx_t i = msb; i != (idx_t)(-1); --i) {
        unsigned int bit = (unsigned int)((n >> i) & 1ULL);
        if (bit) {
            p1 ^= 1;
            result = (result << 1) | (seq_t)p1;
        } else {
            p0 ^= 1;
            result = (result << 1) | (seq_t)p0;
        }
    }
    return result;
}
