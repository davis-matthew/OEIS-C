/*
 * A332769  Permutation of the positive integers: a(n) = A258996(A054429(n)) =
 *          A054429(A258996(n)).
 * https://oeis.org/A332769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by flipping every second bit * (starting from
 *            the least‑significant bit) of n’s binary representation * below
 *            the most significant 1, which remains unchanged. * This
 *            implements OEIS A332769: a(a(n)) = n
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 in the sequence; return 0 for n == 0 as a sentinel. */
    if (n == 0) return (seq_t)0;

    /* Find position of the most significant bit (msb), 0‑based. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        tmp >>= 1;
        ++msb;
    }

    seq_t a_val = 0;

    /* Keep the most significant bit unchanged. */
    a_val |= ((seq_t)1 << msb);

    /* Flip every second bit (starting from LSB) below the msb. */
    for (idx_t i = 0; i < msb; ++i) {
        idx_t bit = (n >> i) & (idx_t)1;   /* original bit i of n     */
        if ((i & 1) == 0)                 /* flip at even positions? */
            bit ^= (idx_t)1;
        if (bit)
            a_val |= ((seq_t)1 << i);     /* set resulting bit i     */
    }

    return a_val;
}
