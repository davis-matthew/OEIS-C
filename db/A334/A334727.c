/*
 * A334727  Binary interpretation of the left diagonal of the XOR-triangle
 *          with first row generated from the binary expansion of n, with most
 *          significant bit given by first row.
 * https://oeis.org/A334727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer whose (k‑1‑r)th bit equals the XOR of *
 *            those bits of n whose indices j satisfy (j & r) == j, with j=0
 *            the * most significant bit of n and r=0 the most significant bit
 *            of a(n)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == (idx_t)0) return (seq_t)0;

    /* Determine the number of bits k of n (k ≥ 1). */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) { ++k; tmp >>= 1; }

    seq_t result = 0;

    for (idx_t r = 0; r < k; ++r)
    {
        seq_t bit = 0;
        for (idx_t j = 0; j < k; ++j)
        {
            if ((j & r) == j)          /* j is a submask of r */
            {
                /* Extract the j‑th bit of n (0 = MSB). */
                seq_t bj = (seq_t)((n >> (k - 1 - j)) & 1);
                bit ^= bj;
            }
        }
        /* Build the output by shifting left and appending the new bit. */
        result = (result << 1) | bit;
    }
    return result;
}
