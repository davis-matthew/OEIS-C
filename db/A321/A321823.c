/*
 * A321823  a(n) = Sum_{d|n, d==1 mod 4} d^7 - Sum_{d|n, d==3 mod 4} d^7.
 * https://oeis.org/A321823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d==1 mod 4} d^7 - Sum_{d|n, d==3 mod 4} d^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 22, i64 n <= 512,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Remove all factors of 2 – they contribute 1 */
    idx_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;
    /* n=1 or n is a power of two */
    if (odd <= 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    /* factor the odd part of n */
    idx_t t = odd;
    /* trial division by odd numbers up to sqrt(t) */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p != 0) continue;
        /* exponent of this prime in n */
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        /* compute p^7 as seq_t */
        seq_t base = (seq_t)p;
        seq_t p7 = base;
        for (int i = 0; i < 6; ++i) p7 *= base;  /* base * base^6 = base^7 */

        /* sum over powers of p^7 depending on p mod 4 */
        seq_t sum = 0;
        seq_t term = (seq_t)1;
        idx_t mod4 = p % 4u;   /* 1 or 3 for an odd prime */
        if (mod4 == 1 /* p ≡ 1 (mod 4) */) {
            for (int k = 0; k <= exp; ++k) {
                sum += term;
                term *= p7;
            }
        } else {  /* p ≡ 3 (mod 4) */
            seq_t sign = (seq_t)1;
            for (int k = 0; k <= exp; ++k) {
                sum += sign * term;
                term *= p7;
                sign = -sign;
            }
        }
        result *= sum;
    }
    /* if any prime factor > sqrt(odd) remains */
    if (t > 1) {
        /* t is a prime with exponent 1 */
        int exp = 1;
        seq_t base = (seq_t)t;
        seq_t p7 = base;
        for (int i = 0; i < 6; ++i) p7 *= base;

        seq_t sum = 0;
        seq_t term = (seq_t)1;
        idx_t mod4 = t % 4u;
        if (mod4 == 1) {
            /* positive geometric series */
            for (int k = 0; k <= exp; ++k) {
                sum += term;
                term *= p7;
            }
        } else {
            /* alternating series */
            seq_t sign = (seq_t)1;
            for (int k = 0; k <= exp; ++k) {
                sum += sign * term;
                term *= p7;
                sign = -sign;
            }
        }
        result *= sum;
    }
    return result;
}
