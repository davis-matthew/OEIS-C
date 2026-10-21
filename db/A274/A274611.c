/*
 * A274611  a(n) = n/8 if A007814(n) == 3 (mod 4), else a(n) = 2*n.
 * https://oeis.org/A274611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/8 if v2(n) == 3 (mod 4), else a(n) = 2*n * Here v2(n)
 *            is the exponent of the largest power of 2 dividing n. * This
 *            implements the OEIS sequence A274611
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute v2(n): the number of trailing zeros in the binary
     * representation of n.  We only need v2(n) modulo 4. */
    idx_t tmp = n;
    idx_t v = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        v += 1;
    }

    /* If v ≡ 3 (mod 4), return n/8; otherwise return 2*n. */
    if ((v & 3) == 3) {          /* v % 4 == 3 */
        return (seq_t)(n >> 3);   /* safe: n is divisible by 8 when v ≥ 3 */
    } else {
        return (seq_t)n * 2;      /* unsigned multiplication wraps modulo width */
    }
}
