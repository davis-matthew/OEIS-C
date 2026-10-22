/*
 * A394477  Number of integers k in 1..n such that the numerator of the
 *          reduced fraction (n*(n-1) + k)/n is prime.
 * https://oeis.org/A394477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ k∈{1..n} | reduced numerator of (n*(n-1)+k)/n is prime
 *            }
 * Range:     u8 n <= 16, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the reduced numerator is (n*(n-1)+k)/gcd(k,n)                     */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* not used, but handle just in case */
        return 0;

    seq_t count = 0;
    seq_t base  = (seq_t)n * (seq_t)(n - 1);   /* n*(n-1) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t g = gcd_u64(k, n);                 /* g = gcd(k,n)         */
        seq_t N = base + (seq_t)k;               /* numerator before reducing */
        seq_t reduced = N / g;                   /* reduced numerator      */
        if (isPrimeGeneric(reduced))
            count++;
    }

    return count;
}
