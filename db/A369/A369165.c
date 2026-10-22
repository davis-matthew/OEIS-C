/*
 * A369165  a(n) = A001222(A000688(n)).
 * https://oeis.org/A369165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct primes p such that p^2 divides n (i.e.
 *            the number of prime factors with exponent >= 2 in the
 *            factorization of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* copy n into a mutable variable of type idx_t, which is unsigned */
    idx_t x = n;
    seq_t cnt = 0;              /* result accumulator */

    /* trial division up to sqrt(x) */
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (exp >= 2) {
                ++cnt;
            }
        }
    }

    /* If after the loop x > 1, it is a prime factor with exponent 1
       (since all factors > sqrt(n) must appear only once). No action needed. */

    return cnt;
}
