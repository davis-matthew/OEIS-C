/*
 * A305614  Expansion of Sum_{p prime} x^p/(1 + x^p).
 * https://oeis.org/A305614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -Sum_{p|n prime} (-1)^(n/p) Simplified: if n is odd, a(n)
 *            = A001221(n) if n ≡ 2 (mod 4), a(n) = 2 - A001221(n) if n ≡ 0
 *            (mod 4) and n>0, a(n) = -A001221(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;

    /* count distinct prime factors of n -- A001221(n) */
    int omega = 0;
    idx_t m = n;

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        omega++;
        while ((m & 1ULL) == 0)
            m >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            omega++;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        omega++;

    /* compute a(n) according to parity of n */
    if ((n & 1ULL) == 1) {           /* n odd */
        return (seq_t)omega;
    } else if ((n % 4) == 2) {       /* n ≡ 2 (mod 4) */
        return (seq_t)(2 - omega);
    } else {                         /* n ≡ 0 (mod 4), n>0 */
        return (seq_t)(-omega);
    }
}
