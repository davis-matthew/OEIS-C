/*
 * A351698  24*a(n)+1 is the numerator of the squared circumradius of a cyclic
 *          quadrilateral with sides n, n+1, n+2, n+3.
 * https://oeis.org/A351698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let t1 = 2n² + 6n + 6, t2 = 2n² + 6n + 3, t3 = 2n² + 6n + 2.
 *            Then N = t1·t2·t3, d = 4 if n mod 3 ≠ 0, = 36 if n mod 3 = 0.
 *            The sequence is a(n) = ( N / d – 1 ) / 24 . This reproduces all
 *            terms of OEIS A351698
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First convert the index to the integer type used for arithmetic. */
    unsigned __int128 sn = (unsigned __int128)n;
    unsigned __int128 t1 = 2*sn*sn + 6*sn + 6;
    unsigned __int128 t2 = 2*sn*sn + 6*sn + 3;
    unsigned __int128 t3 = 2*sn*sn + 6*sn + 2;
    unsigned __int128 N  = t1 * t2 * t3;

    /* Choose the divisor depending on whether 3 divides 2n+3. */
    unsigned __int128 d = ( (n % 3) == 0 ) ? 36 : 4;

    unsigned __int128 reduced = N / d;
    unsigned __int128 val = (reduced - 1) / 24;

    return (seq_t)val;
}
