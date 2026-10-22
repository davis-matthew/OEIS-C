/*
 * A359329  Number of diagonals in a regular polygon with n sides not passing
 *          through the center.
 * https://oeis.org/A359329
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is odd) ? (n^2 - 3*n)/2 : (n^2 - 4*n)/2
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the offset of the sequence is not defined.
       Returning 0 is harmless and keeps the function safe for all
       possible inputs. */
    if (n < 3)
        return (seq_t)0;

    seq_t sn = (seq_t)n;
    if ((sn & (seq_t)1U) == (seq_t)0)  /* even */
        return ((sn * sn) - ((seq_t)4 * sn)) >> 1;
    else                              /* odd */
        return ((sn * sn) - ((seq_t)3 * sn)) >> 1;
}
