/*
 * A359422  Dirichlet inverse of A187074, characteristic function of numbers
 *          that are neither multiples of 3 nor of the form 4u+2.
 * https://oeis.org/A359422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Dirichlet inverse of A187074 (characteristic of numbers
 *            not multiple of 3 and not ≡2 (mod 4)) a(1) = 1 For n > 1: a(n) =
 *            -Sum_{d|n, d<n} A187074(n/d) * a(d) Multiplicative with: a(2^e)
 *            = A010892(e) (period 6: 1,0,-1,-1,0,1 …) a(3^e) = 0 a(p) = -1
 *            for p ≥ 5 a(p^e) = 0 for e > 1, p ≥ 5
 * Range:     i8 n <= 9774, i16 n <= 9774, i32 n <= 9774, i64 n <= 9774,
 *            i128 n <= 9774, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety, offset starts at 1 */
    if (n % 3 == 0) return 0;             /* μ of 3 gives 0 */

    /* factor out powers of two */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        e++;
        m >>= 1;
    }

    /* a(2^e) via period‑6 pattern */
    seq_t res;
    switch (e % 6) {
    case 0: res = 1; break;
    case 1: res = 0; break;
    case 2: res = -1; break;
    case 3: res = -1; break;
    case 4: res = 0; break;
    case 5: res = 1; break;
    default: res = 0; /* unreachable */
    }
    if (res == 0) return 0;   /* any factor already bounds the result */

    if (m == 1) return res;   /* no odd factors left */

    /* trial division over odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp > 1) return 0;  /* a(p^e) = 0 for e > 1, p ≥ 5 */
            res = -res;            /* multiply by -1 for each distinct prime ≥ 5 */
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* m cannot be 3 because n % 3 == 0 was handled earlier */
        res = -res;
    }

    return res;
}
