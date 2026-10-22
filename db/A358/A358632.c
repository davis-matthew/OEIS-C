/*
 * A358632  Coordination sequence for the faces of the uniform infinite
 *          surface that is formed from congruent regular pentagons and from
 *          which there is a continuous function that maps the faces 1:1 to
 *          regular pentagons in the plane.
 * https://oeis.org/A358632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   For even n = 2k (k ≥ 0): a(n) = 35 * k * (k - 1) * (k + 1) / 3 +
 *            20 * k For odd n = 2k + 1 (k ≥ 0): a(n) = (70 * k^3 + 105 * k^2
 *            + 95 * k) / 6 + 5
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for a(0) */
    if (n == 0) {
        return (seq_t)1;
    }

    /* k corresponds to floor(n/2) */
    uint64_t k = n >> 1;

    if ((n & 1) == 0) {            /* even n */
        uint64_t part = 35 * k * (k - 1) * (k + 1);
        uint64_t res  = part / 3 + 20 * k;
        return (seq_t)res;
    } else {                       /* odd n */
        uint64_t part = 70 * k * k * k + 105 * k * k + 95 * k;
        uint64_t res  = part / 6 + 5;
        return (seq_t)res;
    }
}
