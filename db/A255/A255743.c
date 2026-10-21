/*
 * A255743  a(1) = 1; for n > 1, a(n) = 9*8^{A000120(n-1)-1}.
 * https://oeis.org/A255743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9 * 8^{popcount(n-1)-1} (a(1) = 1)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A255743 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Compute popcount of (n-1) */
    idx_t m = n - 1;
    unsigned int ones = 0;
    while (m)
    {
        ones += (unsigned int)(m & 1);
        m >>= 1;
    }

    /* exponent = popcount - 1, always non‑negative for n > 1 */
    seq_t exp = (seq_t)(ones - 1);

    /* Compute 8^exp and multiply by 9 */
    seq_t result = (seq_t)1;
    for (seq_t i = 0; i < exp; ++i)
        result *= (seq_t)8;
    result *= (seq_t)9;

    return result;
}
