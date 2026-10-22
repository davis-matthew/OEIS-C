/*
 * A322465  Numbers on the 0-9-10-line in a spiral on an equilateral
 *          triangular lattice.
 * https://oeis.org/A322465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 6*k^2 + 4*k a(2k+1) = 6*k^2 + 16*k + 9 (where k = n/2
 *            for even n, k = (n-1)/2 for odd n)
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the half-index to seq_t to avoid overflow in idx_t */
    idx_t k_idx = n / 2;          /* floor division */
    seq_t k = (seq_t)k_idx;

    if ((n & 1ULL) == 0) {
        /* even n = 2k */
        return 6 * k * k + 4 * k;
    } else {
        /* odd n = 2k+1, here k = (n-1)/2 as above */
        return 6 * k * k + 16 * k + 9;
    }
}
