/*
 * A291361  The arithmetic function u(n,2,6).
 * https://oeis.org/A291361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime divisor of n among {2,3,5,7}; if no
 *            such divisor exists (n=1 or n has all prime factors ≥11), return
 *            7
 * Range:     u8 n <= 8693, u16 n <= 8693, u32 n <= 8693, u64 n <= 8693,
 *            u128 n <= 8693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 7;                /* offset */
    if ((n & (idx_t)1) == 0) return 2;   /* divisible by 2 */
    if (n % 3 == 0) return 3;            /* divisible by 3 */
    if (n % 5 == 0) return 5;            /* divisible by 5 */
    return 7;                            /* otherwise (including primes >7) */
}
