/*
 * A290254  The viabin numbers of the self-conjugate integer partitions.
 * https://oeis.org/A290254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by concatenating the binary of (n‑1) with the
 *            reversed complement of that binary, then dropping the final bit.
 *            (See the OEIS description of A290254.)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1) {
        return (seq_t)0;
    }

    idx_t m   = n - 1;          /* integer whose bits we transform */
    idx_t tmp = m;
    idx_t len = 0;              /* number of bits in m */

    /* compute bit length of m (m > 0) */
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    seq_t res = 0;

    /* append the original bits of m from most significant to least */
    for (idx_t i = len; i-- > 0; ) {
        res = (res << 1) | (seq_t)((m >> i) & 1);
    }

    /* append flipped bits of m from least significant up to bit len‑2
       (the last reversed‑complement bit is omitted, matching the
       “drop the last bit” rule). */
    for (idx_t i = 0; i + 1 < len; i++) {
        res = (res << 1) | (seq_t)(((m >> i) & 1) ^ 1);
    }

    return res;
}
