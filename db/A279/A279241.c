/*
 * A279241  Let f(n) = 4*n^2 + 2*n + 41. The values |f(n)| are primes for all
 *          n in the range -20 to 19 (but not for n=-21 or 20). The sequence
 *          lists this maximal run of primes in the order in which they
 *          appear.
 * https://oeis.org/A279241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = 4*(n-21)^2 + 2*(n-21) + 41 for 1 ≤ n ≤ 40, 0 otherwise
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to signed to allow subtraction that may go below zero */
    long long k    = (long long)n;
    long long m    = k - 21;                 /* m = n - 21  ↔  n = m + 21  */
    if (m < -20 || m > 19)                  /* outside the maximal run */
        return (seq_t)0;
    long long val  = 4 * m * m + 2 * m + 41; /* 4*m^2 + 2*m + 41  */
    return (seq_t)val;
}
