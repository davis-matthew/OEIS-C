/*
 * A338803  Product of the nonzero digits of (n written in base 5).
 * https://oeis.org/A338803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{d∈digits of n in base 5, d≠0} d (empty product = 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    while (n > 0)
    {
        idx_t d = n % 5;
        if (d != 0)
            prod *= (seq_t)d;
        n /= 5;
    }
    return prod;
}
