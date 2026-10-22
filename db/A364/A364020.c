/*
 * A364020  Expansion of Sum_{k>0} k * x^(2*k) / (1 + x^(5*k)).
 * https://oeis.org/A364020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d == 2 (mod 5)} (-1)^(n/d) * d
 * Range:     i8 n <= 227, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Compute by enumerating all divisors of n and accumulating the terms. */

seq_t a(idx_t n)
{
    /* offset 1: a(1) corresponds to n = 1 */
    if (n == 0) return 0;  /* not used but safe for any idx_t */
    seq_t total = 0;
    idx_t i;

    for (i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;            /* i is not a divisor */
        /* first divisor d1 = i, complementary divisor d2 = n / i */
        idx_t d1 = i;
        idx_t q1 = n / d1;                    /* exponent in (-1)^q1 */
        if (q1 % 5 == 2) {
            seq_t sign1 = ((q1 & 1) == 0) ? 1 : -1;
            total += (seq_t)d1 * sign1;
        }
        idx_t d2 = n / i;
        if (d2 != d1) {                       /* avoid double-counting sqrt(n) */
            idx_t q2 = i;                     /* complementary exponent */
            if (q2 % 5 == 2) {
                seq_t sign2 = ((q2 & 1) == 0) ? 1 : -1;
                total += (seq_t)d2 * sign2;
            }
        }
    }
    return total;
}
