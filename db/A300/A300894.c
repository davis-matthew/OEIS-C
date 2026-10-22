/*
 * A300894  L.g.f.: log(Product_{k>=1} (1 + mu(k)^2*x^k)) = Sum_{n>=1}
 *          a(n)*x^n/n, where mu() is the Moebius function (A008683).
 * https://oeis.org/A300894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 92, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplies the contributions of each prime power.
   Formula: a(n) = ∏_{p odd|n} (p+1)  ·  {-3 if 4|n, 1 otherwise}.
   It covers the special cases:
   a(1)=1, a(2)=1, a(2^e)=1 (e=1) or -3 (e≥2), a(p^e)=p+1 for odd primes. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined in OEIS, safe return */

    idx_t temp = n;
    idx_t e2 = 0;
    /* Count exponent of 2 */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        e2++;
    }

    seq_t result = 1;
    if (e2 >= 2)
        result *= (seq_t)(-3);   /* factor -3 if n divisible by 4 */

    /* Factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            result *= (seq_t)(p + 1);
            while (temp % p == 0)
                temp /= p;        /* skip all powers of this prime */
        }
    }

    /* Any remaining odd prime factor */
    if (temp > 1)
        result *= (seq_t)(temp + 1);

    return result;
}
