/*
 * A383848  In the binary representation of n, rotate left by the number of
 *          ones.
 * https://oeis.org/A383848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   rotate left the binary representation of n by the number of ones
 *            in n. The bit width used is the minimal number of bits needed to
 *            represent n (i.e. floor(log2(n)) + 1). For n = 0 the result is 0
 * Range:     u8 n <= 270, u16 n <= 9247, u32 n <= 9247, u64 n <= 9247,
 *            u128 n <= 9247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */

    /* Count number of 1-bits (ones) and determine bit-width L. */
    idx_t t = n;
    idx_t ones = 0;
    idx_t L = 0;
    while (t) {
        ones += t & 1;
        t >>= 1;
        ++L;
    }

    /* Rotation amount is ones mod L.  If it's zero, the number stays unchanged. */
    idx_t shift = ones % L;
    if (shift == 0)
        return (seq_t)n;

    /* Compute mask for L bits to clip any overflow from the left shift. */
    idx_t mask = (L == 64) ? ~((idx_t)0) : (((idx_t)1 << L) - 1);

    /* Perform circular left shift of n by shift positions within L bits. */
    idx_t rotated = ((n << shift) | (n >> (L - shift))) & mask;

    return (seq_t)rotated;
}
