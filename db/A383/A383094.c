/*
 * A383094  Number of integer partitions of n having exactly one permutation
 *          with all equal run-lengths.
 * https://oeis.org/A383094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer partitions of n having exactly one permutation
   with all equal run-lengths.
   The algorithm counts:
   (1) all-equal partitions – one for each divisor of n;
   (2) partitions with exactly two distinct part values a<b,
       counts c_a and c_b such that
       let g = gcd(c_a,c_b);  let s' = c_a/g, t' = c_b/g;
       |s'‑t'| = 1.
   This second condition is equivalent to the existence of integers
   k≥1, g≥1, a,b≥1 satisfying
       a*(k+1) + b*k = n/g,
   with a ≠ b.  The algorithm enumerates all divisors g of n,
   then for each k≥1 finds all admissible a,b.  The count is
   added to the divisor count.  Complexity is O(n * log n) for
   n up to about 10 000.  The implementation uses only integer
   operations allowed by the specification.  It returns 1 for n==0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    seq_t result = 0;

    /* Count all-equal partitions: one for each divisor of n. */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            result += 1;            /* divisor i */
            if (i != n / i) result += 1;   /* divisor n/i */
        }
    }

    /* Collect all divisors of n for the second part. */
    /* We can reuse the loop above or compute again; this is simple. */
    for (idx_t g = 1; g * g <= n; ++g)
    {
        if (n % g != 0) continue;

        /* Process divisor g. */
        idx_t n_prime = n / g;
        for (idx_t k = 1; ; ++k)
        {
            idx_t u = k + 1;    /* coefficient for a */
            idx_t v = k;        /* coefficient for b */
            if (n_prime < u) break;   /* a_max would be 0 */

            idx_t a_max = n_prime / u;
            for (idx_t a = 1; a <= a_max; ++a)
            {
                idx_t rem = n_prime - a * u;
                if (rem % v != 0) continue;
                idx_t b = rem / v;
                if (b == 0) continue;
                if (a == b) continue;   /* only one part value */
                result += 1;   /* a valid partition of type {a,b} */
            }
        }

        /* Process the paired divisor n/g if distinct. */
        idx_t g2 = n / g;
        if (g2 == g) continue;   /* already processed */

        n_prime = n / g2;
        for (idx_t k = 1; ; ++k)
        {
            idx_t u = k + 1;
            idx_t v = k;
            if (n_prime < u) break;

            idx_t a_max = n_prime / u;
            for (idx_t a = 1; a <= a_max; ++a)
            {
                idx_t rem = n_prime - a * u;
                if (rem % v != 0) continue;
                idx_t b = rem / v;
                if (b == 0) continue;
                if (a == b) continue;
                result += 1;
            }
        }
    }

    return result;
}
