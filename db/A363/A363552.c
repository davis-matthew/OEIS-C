/*
 * A363552  Möbius function of rank 4: a(n) = lambda(n) = A008836(n) if n is
 *          biquadratefree (A046100) and 0 otherwise.
 * https://oeis.org/A363552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = λ4(n) where λ4(p^e) = (-1)^e for e ≤ 3 and 0 for e ≥ 4.
 *            This is the Mobius function of rank 4: λ4(n) = λ(n) if n is
 *            biquadratefree, otherwise 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* λ4(1) = 1 */
    idx_t  temp   = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 3)        /* exponent ≥ 4 gives λ4(p^e) = 0 */
                    return (seq_t)0;
            }
            /* multiply by (-1)^exp */
            if (exp & 1u)
                result = -result;
        }
    }

    /* If a prime > sqrt(original n) remains, its exponent is 1. */
    if (temp > 1)
        result = -result;

    return result;
}
