/*
 * A253195  Numbers congruent to 5 or 8 mod 9.
 * https://oeis.org/A253195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*floor((n-1)/2) + 5 if n is odd, 9*floor((n-1)/2) + 8 if
 *            n is even
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but guard against accidental use */

    /* compute k = floor((n-1)/2) in seq_t */
    seq_t k = ((seq_t)(n - 1)) / 2;

    /* common base part */
    seq_t base = (seq_t)9 * k;

    /* add the appropriate offset */
    if (n & 1ULL)
        return base + (seq_t)5;
    else
        return base + (seq_t)8;
}
