/*
 * A363553  Möbius function of rank 5: a(n) = lambda(n) = A008836(n) if n is
 *          5-free and 0 otherwise.
 * https://oeis.org/A363553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (-1)^k if n is 5‑free with k = sum of prime exponents; 0
 *            otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 5‑free test: if any prime exponent ≥5 --> return 0.
       Otherwise, a(n) is the product of (-1) for each odd exponent. */
    seq_t sign = 1;          /* current product of (-1)^odd_exponent */
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            /* factor out all powers of p */
            while (m % p == 0) {
                ++e;
                m /= p;
                if (e >= 5)        /* exponent too large → not 5‑free */
                    return 0;
            }
            /* flip sign if exponent is odd */
            if (e & 1)
                sign = -sign;
        }
    }

    /* if a prime factor remains > sqrt(original m) it has exponent 1 */
    if (m > 1)
        sign = -sign;

    return sign;   /* 1, -1, or 0 */
}
