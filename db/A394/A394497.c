/*
 * A394497  Recurrence: a(2*n + 1) = 2*n + 1, a(2*n) = 3*a(n).
 * https://oeis.org/A394497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(2*n + 1) = 2*n + 1, a(2*n) = 3*a(n); a(1) = 1
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indexing is 1‑based (a(1) is the first term). */

seq_t a(idx_t n)
{
    /* Count the exponent k such that n = 2^k * m (m odd). */
    idx_t k   = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        k++;
    }

    /* tmp is now odd (the odd part m).  Convert to seq_t. */
    seq_t result = (seq_t)tmp;

    /* Multiply by 3^k.  For big‑integer seq_t this loop stays safe. */
    for (idx_t i = 0; i < k; ++i) {
        result *= 3;
    }

    return result;
}
