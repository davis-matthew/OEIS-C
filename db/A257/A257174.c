/*
 * A257174  a(n) = 4*n/3 if n = 3*k and n!=0, otherwise a(n) = n except a(0) =
 *          1.
 * https://oeis.org/A257174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(0)=1; for n>0: a(n)=n if n%3!=0 else (4*n)/3
 * Range:     u8 n <= 191, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if ((n % 3) == 0)
        return ((seq_t)n * 4) / 3;
    return (seq_t)n;
}
