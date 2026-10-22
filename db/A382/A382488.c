/*
 * A382488  The number of unitary 3-smooth divisors of n.
 * https://oeis.org/A382488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Number of unitary 3‑smooth divisors of n = 2^k where k = number
 *            of distinct primes from {2,3} dividing n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int cnt = 0;
    if (n % 2 == 0) cnt++;   /* prime 2 present */
    if (n % 3 == 0) cnt++;   /* prime 3 present */
    return ((seq_t)1 << cnt);
}
