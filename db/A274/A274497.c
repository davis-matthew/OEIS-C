/*
 * A274497  Sum of the degrees of asymmetry of all binary words of length n.
 * https://oeis.org/A274497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n - (n%2)) * 2^(n-2) for n >= 2, a(0)=0, a(1)=0
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2u)
        return (seq_t)0;

    /* compute 2^(n-2) by repeated multiplication */
    idx_t exp = n - 2;
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        pow *= (seq_t)2;
    }

    /* factor = n - (n%2)  (i.e., n if n is even, n-1 if n is odd) */
    seq_t factor = (seq_t)(n - (n & 1u));

    return factor * pow;
}
