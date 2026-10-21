/*
 * A246520  Largest number that can be written in binary as concatenation of
 *          (n - k) and (n + k), 0 <= k <= n.
 * https://oeis.org/A246520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max_{0<=k<=n} ((n-k) concatenated with (n+k))
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The concatenation is performed in binary, i.e. the binary string of (n-k)
   is placed before the binary string of (n+k). */

seq_t a(idx_t n)
{
    /* Special case for n = 0: the only choice is k = 0, giving 0. */
    if (n == 0)
        return 0;

    seq_t best = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Parts to be concatenated */
        seq_t a_part = (seq_t)n - (seq_t)k;   /* n - k  (>= 0)          */
        seq_t b_part = (seq_t)n + (seq_t)k;   /* n + k  (>= 0)          */

        /* Bit length of b_part (number of bits needed to represent it) */
        idx_t  t   = 0;
        seq_t  tmp = b_part;
        while (tmp) { tmp >>= 1; ++t; }

        /* Concatenate: shift a_part left by t bits and OR with b_part */
        seq_t val = (a_part << t) | b_part;

        if (val > best)
            best = val;
    }

    return best;
}
