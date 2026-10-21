/*
 * A276833  Sum of mu(d)*phi(d) over divisors d of n.
 * https://oeis.org/A276833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign, easy
 * Formula:   a(n) = Π_{p|n} (2 – p) (product over distinct prime divisors p
 *            of n; for n = 1, a(1) = 1)
 * Range:     i8 n <= 130, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) is the only special case. */
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Handle factor 2: if 2 divides n then (2-2) = 0, so a(n)=0. */
    if ((m & 1) == 0) return (seq_t)0;

    /* Trial division by odd primes. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            result *= ((seq_t)2 - (seq_t)p);
            while (m % p == 0) m /= p;
        }
    }

    /* If a prime factor remains after the loop it is > sqrt(original n). */
    if (m > 1) {
        result *= ((seq_t)2 - (seq_t)m);
    }

    return result;
}
