/*
 * A358380  a(n) = Sum_{d|n} tau(d^5), where tau(n) = number of divisors of n,
 *          cf. A000005.
 * https://oeis.org/A358380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} tau(d^5) (number of divisors of d^5)
 * Range:     u8 n <= 29, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* outside the OEIS offset – harmless */

    idx_t tmp = n;
    seq_t result = 1;                 /* multiplicative identity */

    /* trial division up to sqrt(tmp) */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t e = exp;
            /* local factor for prime power p^exp:
               f(p^e) = (5e^2 + 7e + 2)/2  (guaranteed integer) */
            seq_t local = (5 * e * e + 7 * e + 2) / 2;
            result *= local;
        }
    }

    /* any remaining prime factor has exponent 1 */
    if (tmp > 1) {
        seq_t local = 7;              /* f(p) = (5 + 7 + 2)/2 = 7 */
        result *= local;
    }

    return result;
}
