/*
 * A359591  Dirichlet inverse of A035263, where A035263(n) is parity of 2-adic
 *          valuation of 2n.
 * https://oeis.org/A359591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 0 if the exponent of 2 in n is 1 or ≥3; otherwise, if n
 *            is odd and square‑free, a(n)=(-1)^{ω(n)}; if n = 4·m with m odd
 *            square‑free, a(n)=(-1)^{ω(m)+1}
 * Range:     i8 n <= 9659, i16 n <= 9659, i32 n <= 9659, i64 n <= 9659,
 *            i128 n <= 9659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative with a(2^e) = -1 if e=2, 0 otherwise; 
   a(p^e) = -1 if e=1 for odd primes, 0 otherwise. */
seq_t a(idx_t n)
{
    /* Count exponent of 2 */
    idx_t e2 = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }

    /* If exponent of 2 is 1 or ≥3, value is zero */
    if (e2 == 1 || e2 >= 3)
        return (seq_t)0;

    /* m is now odd. Factor m and ensure it is square‑free. */
    idx_t temp = m;
    idx_t omega = 0; /* number of distinct odd prime factors */

    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            do {
                temp /= p;
                cnt++;
            } while (temp % p == 0);
            if (cnt > 1)          /* square factor found */
                return (seq_t)0;
            omega++;              /* each distinct prime contributes -1 */
        }
    }
    if (temp > 1)                  /* remaining prime factor */
        omega++;

    /* Sign determination */
    seq_t sign = (omega & 1) ? -1 : 1;   /* (-1)^omega for odd part   */
    if (e2 == 2)                     /* extra -1 from factor 4    */
        sign = -sign;
    return sign;
}
