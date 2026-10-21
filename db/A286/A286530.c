/*
 * A286530  a(n) = d(n+d(n)) - d(n), where d(n) is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A286530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = d(n+d(n)) - d(n)
 * Range:     i8 n <= 2476, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: return the number of positive divisors of n. */
static inline seq_t divisor_count(idx_t n)
{
    seq_t cnt = 0;
    /* iterate i from 1 to floor(sqrt(n)) */
    for (idx_t i = 1; (seq_t)i * (seq_t)i <= (seq_t)n; ++i)
    {
        if (n % i == 0)
        {
            /* i divides n; count one divisor if i==n/i, else two */
            if (i == n / i)
                ++cnt;
            else
                cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not defined for n==0; return 0 as a safe value */

    seq_t dn = divisor_count(n);          /* d(n) */
    idx_t m = n + (idx_t)dn;               /* n + d(n) */
    seq_t dm = divisor_count(m);          /* d(n + d(n)) */

    return dm - dn;                        /* a(n) = d(n+d(n)) - d(n) */
}
