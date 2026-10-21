/*
 * A257921  Expansion of f(x^2, -x^4) * f(-x, -x^5)^2 / f(-x^12, -x^12) in
 *          powers of x where f(, ) is Ramanujan's general theta functions.
 * https://oeis.org/A257921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(4*n + 3), where b is multiplicative with b(3^e) = 1,
 *            b(p^e) = e+1 if p ≡ 1,11 (mod 24), b(p^e) = (e+1)*(-1)^e if p ≡
 *            5,7 (mod 24), b(p^e) = (1+(-1)^e)/2 if p ≡ 13,17,19,23 (mod 24)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 4*n + 3 (m is always odd). */
    idx_t m = 4*n + 3;
    seq_t result = 1;

    /* Factor m by trial division. */
    idx_t temp = m;

    /* Handle small factors from 2 upward; 2 never divides m because m is odd. */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0) continue;
        /* Count exponent of prime p. */
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        seq_t value = 1;          /* default multiplicative value */
        int mod24 = (int)(p % 24);

        if (p == 3) {
            value = 1;
        } else if (mod24 == 1 || mod24 == 11) {
            value = exp + 1;
        } else if (mod24 == 5 || mod24 == 7) {
            value = exp + 1;
            if (exp & 1) value = -value;        /* multiply by -1 for odd e */
        } else if (mod24 == 13 || mod24 == 17 ||
                   mod24 == 19 || mod24 == 23) {
            if (exp & 1) return 0;              /* odd exponent gives zero */
            value = 1;                          /* even exponent gives 1 */
        } else {
            /* This should not occur for odd primes not in any case above */
            value = 1;
        }

        result *= value;
        if (result == 0) return 0;              /* early exit */
    }

    /* If temp > 1, it is a prime factor left. */
    if (temp > 1) {
        idx_t p = temp;
        idx_t exp = 1;
        seq_t value = 1;
        int mod24 = (int)(p % 24);

        if (p == 3) {
            value = 1;
        } else if (mod24 == 1 || mod24 == 11) {
            value = exp + 1;
        } else if (mod24 == 5 || mod24 == 7) {
            value = exp + 1;
            if (exp & 1) value = -value;
        } else if (mod24 == 13 || mod24 == 17 ||
                   mod24 == 19 || mod24 == 23) {
            if (exp & 1) return 0;
            value = 1;
        } else {
            value = 1;
        }

        result *= value;
    }

    return result;
}
