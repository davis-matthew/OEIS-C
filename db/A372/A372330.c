/*
 * A372330  The number of divisors of the smallest multiple of n whose number
 *          of divisors is a power of 2 (A036537).
 * https://oeis.org/A372330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} 2^ceil(log2(e+1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function: a(p^e) = 2^ceil(log2(e+1)) */

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;   /* undefined for n < 1, but guard */
    }

    seq_t result = 1;
    idx_t tmp = n;

    /* trial division up to sqrt(tmp) */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int count = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++count;
            }
            /* count is exponent e */
            uint64_t eplus = (uint64_t)count + 1;   /* e+1 */
            /* find smallest t with 2^t >= e+1 */
            int t = 0;
            while ((1ULL << t) < eplus) {
                ++t;
            }
            result *= (seq_t)(1ULL << t);
        }
    }

    /* if any prime factor > sqrt(original n) remains */
    if (tmp > 1) {
        /* remaining prime exponent is 1, e+1 = 2, ceil(log2(2)) = 1 */
        result *= (seq_t)2;
    }

    return result;
}
