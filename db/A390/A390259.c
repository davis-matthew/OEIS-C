/*
 * A390259  Expansion of Sum_{k>=1} (-1)^(k+1)*x^(k^3)/(1-x^(k^3)).
 * https://oeis.org/A390259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = ∑_{d^3 | n} (-1)^(d+1) (number of odd cubes dividing n
 *            minus the number of even cubes dividing n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        e2++;
    }

    seq_t result = 1;
    result *= ((seq_t)1 - (seq_t)(e2 / 3));   /* factor for the 2‑part */

    for (idx_t p = 3; p <= temp / p; p += 2)  /* factor odd primes */
    {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            result *= ((seq_t)1 + (seq_t)(e / 3));
        }
    }
    /* any remaining prime factor has exponent 1 and contributes (1+0)=1,
       so it can be ignored */
    return result;
}
