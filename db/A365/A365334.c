/*
 * A365334  The sum of exponentially odd divisors of the largest square
 *          dividing n.
 * https://oeis.org/A365334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of “exponentially odd” divisors of the largest square
 *            dividing n * (OEIS A365334). An exponentially odd divisor d of a
 *            number m is a divisor * whose exponent of each prime in its
 *            factorisation is either 0 or odd. * * The largest square
 *            dividing n is ∏ p^{2 ⌊e_p/2⌋} where e_p is the exp...
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 for all calls with the OEIS offset 1 */
    if (n < 1)
        return 0; /* Undefined, but keep it safe */

    seq_t result = (seq_t)1;          /* multiplicative identity   */
    idx_t temp   = n;                 /* number to factorise       */

    /* trial division – works fast for n up to about 10⁶, and fine for the required limits */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* count exponent of prime p in n */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e >= 2) {
                /* t = floor(e/2)  → exponent of p in the largest square */
                idx_t t = e / 2;
                /* sum of odd powers of p up to p^(2t-1) */
                seq_t sum = (seq_t)1;          /* the 1 from exponent 0 */
                seq_t term = (seq_t)p;        /* current odd power */
                for (idx_t i = 0; i < t; ++i) {
                    sum += term;
                    term *= (seq_t)p;        /* exponent +1 → even */
                    term *= (seq_t)p;        /* exponent +1 → next odd */
                }
                result *= sum;
            }
        }
    }

    /* if anything remains, it is a prime > sqrt(original n) with exponent 1,
       thus not contributing to the largest square → ignore  */
    return result;
}
