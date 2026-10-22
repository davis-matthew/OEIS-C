/*
 * A384055  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is odd.
 * https://oeis.org/A384055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e||n} m(p^e) where m(2^e) = 2^e - 1 m(p^e) = p^e for
 *            odd primes p
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t rem = n;

    /* handle factor 2 */
    if (rem % 2 == 0) {
        idx_t e = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            e++;
        }
        seq_t term = 1;
        for (idx_t i = 0; i < e; ++i)
            term *= 2;            /* 2^e */
        term -= 1;                /* 2^e - 1 */
        result *= term;
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            idx_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                e++;
            }
            seq_t term = 1;
            for (idx_t i = 0; i < e; ++i)
                term *= p;        /* p^e */
            result *= term;
        }
    }

    /* any remaining prime factor > sqrt(rem) */
    if (rem > 1) {
        /* rem is prime with exponent 1 */
        result *= rem;
    }

    return result;
}
