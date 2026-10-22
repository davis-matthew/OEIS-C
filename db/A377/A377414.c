/*
 * A377414  a(n) is the largest term of A126684, say b, such that n AND b = b
 *          (where AND denotes the bitwise AND operator).
 * https://oeis.org/A377414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n with bits at even positions from the left set to 0. The
 *            value equals n AND A000975(A070939(n)) and satisfies the
 *            recurrence a(2·n) = 2·a(n). Here we compute it by keeping, for
 *            each bit position p (counted from 0 at the least‑significant
 *            bit), those bits for which (p + bitlen) is odd, whe...
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Determine the number of bits needed to represent n */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++bitlen;
    }

    seq_t result = 0;
    for (idx_t p = 0; p < bitlen; ++p) {
        /* Check whether n has the bit p set */
        idx_t bit_mask_idx = ((idx_t)1 << p);
        if ((n & bit_mask_idx) == 0) {
            continue;
        }

        /* Keep the bit only if (p + bitlen) is odd */
        if (((p + bitlen) & 1) == 1) {
            result |= ((seq_t)1 << p);
        }
    }
    return result;
}
