/*
 * A303814  Generalized 24-gonal (or icositetragonal) numbers: m*(11*m - 10)
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k * (11*k - 10) if n is odd, * a(n) = k * (11*k + 10) if
 *            n is even, * where k = (n + 1) / 2. * This implements the
 *            generalized 24‑gonal (icositetragonal) numbers * A303814 without
 *            using a recurrence or a precomputed table
 * Range:     u8 n <= 9, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = (n + 1) / 2 */
    seq_t k = (seq_t)((n + 1) >> 1);

    if (n & 1u)  /* odd index → positive m = +k */
    {
        return k * ((seq_t)11 * k - (seq_t)10);
    }
    else  /* even index → negative m = -k */
    {
        return k * ((seq_t)11 * k + (seq_t)10);
    }
}
