/*
 * A374324  The maximal exponent in the prime factorization of the numbers
 *          whose maximal exponent in their prime factorization is even.
 * https://oeis.org/A374324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A051903(A368714(n)) (the exponent of the largest prime
 *            power dividing the nth integer whose maximum exponent in its
 *            prime factorization is even). Implemented by scanning integers
 *            and selecting those with even maximum exponent
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;       /* how many qualifying numbers seen so far */

    for (idx_t m = 1; ; ++m) {         /* iterate over all natural numbers */
        idx_t temp   = m;
        idx_t maxExp = 0;               /* largest exponent in the factorisation */

        /* factorise temp by trial division */
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                idx_t c = 0;
                do {
                    temp /= p;
                    ++c;
                } while (temp % p == 0);
                if (c > maxExp) maxExp = c;
            }
        }
        /* if a prime factor larger than sqrt remains, its exponent is 1 */
        if (temp > 1) {
            if (1 > maxExp) maxExp = 1;
        }

        /* accept numbers whose maximal exponent is even */
        if ((maxExp & 1) == 0) {
            ++count;
            if (count == n) return (seq_t)maxExp;
        }
    }
}
