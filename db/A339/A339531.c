/*
 * A339531  Numbers b > 1 such that the smallest two primes, i.e., 2 and 3 are
 *          base-b Wieferich primes.
 * https://oeis.org/A339531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*floor((9*n)/2)+1
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor((9*n)/2) in seq_t type and then apply the formula */
    seq_t t = ((seq_t)9 * n) / 2;  /* floor division */
    return t * 4 + 1;
}
