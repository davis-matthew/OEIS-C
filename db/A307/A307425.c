/*
 * A307425  Dirichlet g.f.: zeta(s) / (zeta(2*s) * zeta(3*s)).
 * https://oeis.org/A307425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A307425.
   The function is multiplicative with:
       a(p)         =  1
       a(p^e)       = -1 if e = 3 or 4
                      0 if e = 2 or e ≥ 5
   For n ≥ 1 a(n) is computed by prime factorisation of n. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0, but return 0 safely */
        return 0;
    if (n == 1)
        return 1;

    seq_t sign = 1;           /* current value, 1, -1 or 0 */
    idx_t m = n;

    /* factorisation of the power of two */
    idx_t cnt = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++cnt;
    }
    if (cnt > 0) {
        if (cnt == 2 || cnt >= 5)  /* exponent 2 or ≥5 → 0 */
            return 0;
        if (cnt == 3 || cnt == 4)  /* exponent 3 or 4 → -1 */
            sign = -sign;
    }

    /* factorise odd primes up to √m */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            if (cnt == 2 || cnt >= 5)
                return 0;
            if (cnt == 3 || cnt == 4)
                sign = -sign;
        }
    }

    /* any remaining factor m > 1 is prime with exponent 1; sign unchanged */
    return sign;
}
