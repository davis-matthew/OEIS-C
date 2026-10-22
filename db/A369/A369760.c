/*
 * A369760  The number of divisors of the smallest multiple of n whose prime
 *          factorization exponents are all powers of 2.
 * https://oeis.org/A369760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over primes p^e dividing n of (2^ceil(log2(e)) +
 *            1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the number of divisors of the smallest multiple of n
   whose prime‑factor exponents are all powers of two. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, safe guard */

    seq_t result = 1;
    idx_t tmp = n;

    /* handle factor 2 separately */
    if ((tmp & 1) == 0) {
        unsigned int exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++exp;
        }
        if (exp > 0) {
            unsigned int power = 1;
            while (power < exp) power <<= 1;  /* smallest 2^k >= exp */
            result *= (seq_t)(power + 1);
        }
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            unsigned int power = 1;
            while (power < exp) power <<= 1;
            result *= (seq_t)(power + 1);
        }
    }

    /* any remaining prime factor has exponent 1 */
    if (tmp > 1) {
        result *= 2;   /* 2^ceil(log2(1)) + 1 = 2 */
    }

    return result;
}
