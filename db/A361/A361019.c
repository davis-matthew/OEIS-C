/*
 * A361019  Dirichlet inverse of A038712.
 * https://oeis.org/A361019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1. For n > 1, a(n) is multiplicative with a(2) = -3,
 *            a(2^2) = 2, a(2^e) = 0 for e > 2 a(p) = -1 for odd prime p,
 *            a(p^e) = 0 for e > 1 In particular a(n) is the product of the
 *            values at the prime‑power factors of n
 * Range:     i8 n <= 9701, i16 n <= 9701, i32 n <= 9701, i64 n <= 9701,
 *            i128 n <= 9701, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    seq_t result = 1;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {                /* p is a prime divisor of n */
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            seq_t factor;
            if (p == 2) {
                if (exp == 1)
                    factor = (seq_t)-3;
                else if (exp == 2)
                    factor = (seq_t) 2;
                else
                    factor = (seq_t) 0;     /* exp > 2 */
            } else {                        /* odd prime */
                if (exp == 1)
                    factor = (seq_t)-1;
                else
                    factor = (seq_t) 0;     /* exp > 1 */
            }

            result *= factor;
            if (result == 0)
                return 0;                   /* any zero factor zeroes the product */
        }
    }

    /* Remaining factor, if any, is a prime with exponent 1. */
    if (temp > 1) {
        seq_t factor = (temp == 2 ? (seq_t)-3 : (seq_t)-1);
        result *= factor;
    }

    return result;
}
