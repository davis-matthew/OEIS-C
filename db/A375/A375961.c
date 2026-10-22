/*
 * A375961  2-adic valuation of 6*n + 2.
 * https://oeis.org/A375961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v₂(6*n + 2) = 1 + v₂(3*n + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the exponent of 2 dividing 3*n + 1 */
    unsigned int v = 0;
    for (unsigned k = 1; k <= 64; ++k) {
        idx_t mask = (k == 64) ? ~0ULL : ((1ULL << k) - 1);
        idx_t t = ((n & mask) * 3ULL + 1ULL) & mask;
        if (t == 0)
            ++v;
        else
            break;
    }
    return (seq_t)(v + 1);   /* a(n) = 1 + v₂(3*n + 1) */
}
