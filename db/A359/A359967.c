/*
 * A359967  a(n) = Sum_{d|n, d+1 is square} d.
 * https://oeis.org/A359967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | n, d+1 is a perfect square} d
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned integers.
   Returns floor(sqrt(n)). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t a = 0;
    /* high bound: 2^((bitcount(n)+1)/2) is an upper bound for sqrt(n) */
    idx_t b = 1ULL << ((sizeof(idx_t) * 8 + 1) / 2);
    while (b > a + 1) {
        idx_t mid = (a + b) >> 1;
        /* avoid overflow: mid*mid <= n  <=>  mid <= n/mid  (mid>0) */
        if (mid > 0 && mid <= n / mid)
            a = mid;
        else
            b = mid;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0)
            continue;

        /* Check d */
        idx_t t = d + 1;
        idx_t s = isqrt_u64(t);
        if (s * s == t)
            sum += (seq_t)d;

        /* Check paired divisor e = n/d, avoid double counting square root */
        idx_t e = n / d;
        if (e != d) {
            t = e + 1;
            s = isqrt_u64(t);
            if (s * s == t)
                sum += (seq_t)e;
        }
    }
    return sum;
}
