/*
 * A323547  n-th digit in the base-2 expansion of 1/n.
 * https://oeis.org/A323547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th digit after the binary point of 1/n.
   a(n) is the digit at position n in the binary expansion of 1/n. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;         /* denominator in seq_t */
    seq_t r  = (seq_t)1 % sn;    /* initial remainder (1 mod n) */
    seq_t digit = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        r      = r * 2;
        digit  = r / sn;
        r      = r % sn;
    }
    return digit;
}
