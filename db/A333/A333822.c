/*
 * A333822  Number of ways to write n as the difference of two k-gonal numbers
 *          for k >= 3.
 * https://oeis.org/A333822
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as the difference of two
 *            k‑gonals for k ≥ 3. A k‑gon number is P(k,m) =
 *            ((k-2)m^2-(k-4)m)/2, with m ≥ 0. We count triples (k,m1,m2) with
 *            m1>m2≥0 such that P(k,m1)-P(k,m2) = n. The implementation
 *            enumerates all divisors of 2n and checks the Diophantine
 *            equatio...
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t count = 0;
    const idx_t N2 = 2 * n;          /* 2n as unsigned integer */

    /* Enumerate all positive divisors s of 2n. */
    for (idx_t i = 1; i * i <= N2; ++i) {
        if (N2 % i != 0) continue;
        idx_t s1 = i;
        idx_t s2 = N2 / i;

        /* Process divisor s1 */
        {
            idx_t s = s1;
            if (s == 1) {
                /* s = 1  ->  n = C*t + 1  (C = k-2, t = m2) */
                idx_t m = n - 1;
                for (idx_t d = 1; d * d <= m; ++d) {
                    if (m % d == 0) {
                        ++count;               /* divisor d  (C = d) */
                        if (d != m / d) ++count;
                    }
                }
            } else {
                idx_t M = N2 / s;          /* Integer division:  M = 2n / s */
                if (M > 2) {
                    idx_t bound = (M - 2) / (s - 1);   /* maximal C */
                    for (idx_t C = 1; C <= bound; ++C) {
                        /* numerator in the equation:
                           2n = s * (2C*t + C*s - C + 2)
                           => num = M - C*s + C - 2 = 2C*t
                        */
                        int64_t num = (int64_t)M - (int64_t)C * (int64_t)s
                                      + (int64_t)C - 2;
                        if (num < 0) continue;   /* t would be negative */
                        idx_t denom = 2 * C;
                        if (num % denom == 0) {
                            /* t = num / (2C) ;  t may be zero (m2 = 0) */
                            ++count;
                        }
                    }
                }
            }
        }

        /* If s2 is distinct from s1, process divisor s2 */
        if (s2 != s1) {
            idx_t s = s2;
            if (s == 1) {
                idx_t m = n - 1;
                for (idx_t d = 1; d * d <= m; ++d) {
                    if (m % d == 0) {
                        ++count;
                        if (d != m / d) ++count;
                    }
                }
            } else {
                idx_t M = N2 / s;
                if (M > 2) {
                    idx_t bound = (M - 2) / (s - 1);
                    for (idx_t C = 1; C <= bound; ++C) {
                        int64_t num = (int64_t)M - (int64_t)C * (int64_t)s
                                      + (int64_t)C - 2;
                        if (num < 0) continue;
                        idx_t denom = 2 * C;
                        if (num % denom == 0) {
                            ++count;
                        }
                    }
                }
            }
        }
    }
    return count;
}
