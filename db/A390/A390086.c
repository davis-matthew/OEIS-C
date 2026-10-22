/*
 * A390086  a(n) is the smallest index k such that A390084(k) = n.
 * https://oeis.org/A390086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n*(n+3))/6 for all n except when n mod 12 == 6, in which
 *            case a(n) = (n*(n+3))/6 - 1. This matches the OEIS definition of
 *            A390086 (the minimal plateau start for the irreducible zero-sum
 *            integer set of length n)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(n*(n+3)/6) */
    seq_t t = ((seq_t)n * ((seq_t)n + (seq_t)3)) / (seq_t)6;
    /* special correction for n ≡ 6 (mod 12) */
    if (n % (idx_t)12 == (idx_t)6) {
        /* subtraction is safe: t > 0 for n ≥ 1 and n%12==6 only occurs when n≥6 */
        t -= (seq_t)1;
    }
    return t;
}
