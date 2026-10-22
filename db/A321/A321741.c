/*
 * A321741  Product of the first n terms of A007318 (Pascal), read as a
 *          sequence.
 * https://oeis.org/A321741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 24, u64 n <= 34, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321741: a(n) = product_{j=1..n} A007318(j) */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t count = 0;

    for (idx_t row = 0; count < n; ++row)
    {
        /* First element of the row: C(row, 0) = 1 */
        prod *= (seq_t)1;
        ++count;
        if (count == n)
            break;

        seq_t val = (seq_t)1;
        for (idx_t k = 1; k <= row; ++k)
        {
            /* Compute C(row, k) from C(row, k-1) */
            val = val * (seq_t)(row - k + 1) / (seq_t)k;
            prod *= val;
            ++count;
            if (count == n)
                break;
        }
    }

    return prod;
}
