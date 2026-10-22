/*
 * A365401  The number of divisors of the largest unitary divisor of n that is
 *          a square.
 * https://oeis.org/A365401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e||n, e even} (e+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of divisors of the largest unitary divisor of n that is a square */

/* The function is multiplicative: for each prime power p^e dividing n,
   if e is odd it contributes 1, otherwise it contributes (e+1). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n≥1 by specification, but guard anyway */
    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 separately to allow skipping even numbers later */
    if ((m & 1) == 0) {                /* m even? */
        idx_t exp = 0;
        while ((m & 1) == 0) {        /* while m divisible by 2 */
            m >>= 1;
            exp++;
        }
        if ((exp & 1) == 0)           /* exponent even? */
            result *= (seq_t)(exp + 1);
    }

    idx_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if ((exp & 1) == 0)       /* exponent even? */
                result *= (seq_t)(exp + 1);
        }
        p += 2;                        /* test only odd primes */
    }

    /* if remaining m>1 it is a prime factor with exponent 1 (odd) */
    /* odd exponent contributes 1, so nothing to multiply */

    return result;
}
