/*
 * A354958  Coordination sequence for the Manhattan hexagonal lattice with
 *          respect to the point (X=0, Y=1).
 * https://oeis.org/A354958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1, * a(1) = 3, * for even n >= 2: a(n) = 18 * (n/2) -
 *            9, * for odd n >= 3: a(n) = 6 * ((n-1)/2) + 9
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 3;

    if ((n & 1u) == 0u) {            /* even n >= 2 */
        idx_t k = n / 2u;
        seq_t res = (seq_t)18 * (seq_t)k;
        res -= 9;
        return res;
    } else {                          /* odd n >= 3 */
        idx_t k = (n - 1u) / 2u;
        seq_t res = (seq_t)6 * (seq_t)k;
        res += 9;
        return res;
    }
}
