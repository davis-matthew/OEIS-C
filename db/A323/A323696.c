/*
 * A323696  a(n) = [x^(n*(n+2))] Sum_{m>=0} x^m * (1 - x^(m+1))^m / (1 +
 *          x^(m+1))^(m+1).
 * https://oeis.org/A323696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n*(n+2))] Sum_{m>=0} x^m * (1 - x^(m+1))^m / (1 +
 *            x^(m+1))^(m+1)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 26, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      a(n) is obtained as the coefficient of x^(n*(n+2)) in the series G(x).  */
/* This implementation evaluates a(n) by exploiting the divisor structure of */
/* (n+1)^2: a(n) = Σ_{d | (n+1)^2} (-1)^{( (n+1)^2 / d ) - 1} * S(d-1, ( (n+1)^2 / d ) - 1), */
/* where S(m,k) = Σ_{i=0}^{min(m,k)} C(m,i) * C(m+k-i, k-i).                     */
/* The sums are evaluated with incremental binomial computations.              */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The exponent to extract */
    idx_t m1 = n + 1;                     /* m1 = n+1 */
    idx_t M  = m1 * m1;                   /* M = (n+1)^2, fits in 64‑bit */

    seq_t result = 0;

    /* Enumerate all divisors d of M */
    for (idx_t d = 1; d * d <= M; ++d) {
        if (M % d != 0) continue;

        /* Process divisor d */
        idx_t  t   = M / d;                 /* t = M / d  */
        seq_t  k   = (seq_t)t - 1;          /* k = t - 1  */
        seq_t  m   = (seq_t)d - 1;          /* m = d - 1  */

        /* Compute S(m,k) via incremental binomials */
        idx_t   lim = (m < k) ? (idx_t)m : (idx_t)k;   /* iterate i=0..lim */
        seq_t  Cmi = 1;                      /* binom(m,i)  */
        seq_t  C2i = 1;                      /* binom(m+k-i, k-i)  */

        /* Compute C2i for i = 0 :  binom(m+k, m)  (use m terms) */
        if (m > 0) {
            for (idx_t t2 = 1; t2 <= m; ++t2) {
                C2i = C2i * (k + t2);
                C2i = C2i / t2;
            }
        } else {
            C2i = 1;           /* binom(k,0) = 1 */
        }

        seq_t S = 0;
        for (idx_t i = 0; i <= lim; ++i) {
            seq_t term = Cmi * C2i;
            S += term;

            if (i == lim) break;

            /* Update binomial Cmi -> C(m, i+1) */
            Cmi = Cmi * (m - i);
            Cmi = Cmi / (i + 1);

            /* Update binomial C2i -> binom(m+k-(i+1), k-(i+1)) */
            seq_t num = k - i;
            seq_t den = m + k - i;
            C2i = C2i * num;
            C2i = C2i / den;
        }

        /* Add or subtract S depending on the parity of t-1 */
        if (((t - 1) & 1) == 0) {
            result += S;
        } else {
            result -= S;
        }

        /* If d is not the square root, process its pair M/d */
        if (d != M / d) {
            idx_t  d2  = M / d;
            t = M / d2;
            k = (seq_t)t - 1;
            m = (seq_t)d2 - 1;

            /* Compute S(m,k) again */
            lim = (m < k) ? (idx_t)m : (idx_t)k;
            Cmi = 1;
            C2i = 1;
            if (m > 0) {
                for (idx_t t2 = 1; t2 <= m; ++t2) {
                    C2i = C2i * (k + t2);
                    C2i = C2i / t2;
                }
            } else {
                C2i = 1;
            }
            S = 0;
            for (idx_t i = 0; i <= lim; ++i) {
                seq_t term = Cmi * C2i;
                S += term;

                if (i == lim) break;

                Cmi = Cmi * (m - i);
                Cmi = Cmi / (i + 1);

                seq_t num = k - i;
                seq_t den = m + k - i;
                C2i = C2i * num;
                C2i = C2i / den;
            }

            if (((t - 1) & 1) == 0) {
                result += S;
            } else {
                result -= S;
            }
        }
    }

    return result;
}
