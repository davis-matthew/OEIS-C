/*
 * A335583  Number of 1's in the partitions of n into 4 parts.
 * https://oeis.org/A335583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n*n + 4*n + 6*[(n mod 6)==4]) / 12) + (n==4 ? 1 :
 *            0) For n ≤ 3 , a(n) = 0. This implements the closed form given
 *            in the OEIS entry
 * Range:     u8 n <= 53, u16 n <= 884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    /* Compute the numerator of the fraction */
    idx_t mod6   = n % 6;
    idx_t numer  = n * n + 4 * n + (mod6 == 4 ? 6 : 0);

    /* Integer division gives the floor */
    idx_t val    = numer / 12;

    /* Add the extra 1 for n = 4 (the [n<5] term in the formula) */
    if (n == 4)
        val += 1;

    return (seq_t)val;
}
