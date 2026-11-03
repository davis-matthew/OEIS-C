/*
 * A307893  a(n) = lcm(sigma(n), pod(n)) / n, where sigma (k) = the sum of
 *          divisors of k (A000203) and pod(n) = the product of divisors of k
 *          (A007955).
 * https://oeis.org/A307893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(sigma(n), pod(n)) / n
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 47, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   gcd, ipow_i32, ipow_idx, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utility functions */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Sum of divisors */
    seq_t sigma = sigma_of(n);

    /* Number of divisors */
    size_t d = num_divisors(n);

    /* Product of divisors = n^{d/2}  (integer) */
    seq_t pod;
    if ((d & 1) == 0) {                     /* d even */
        pod = ipow_idx((seq_t)n, (idx_t)(d / 2));
    } else {                                 /* d odd – n is a perfect square */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        pod = ipow_idx(sqrt_n, (idx_t)d);     /* sqrt_n^{d} = n^{d/2} */
    }

    /* Least common multiple of sigma and pod */
    seq_t g = gcd(sigma, pod);
    seq_t l = seq_mul(sigma, pod);
    l = l / g;                               /* lcm(sigma, pod) */

    /* Divide by n to obtain a(n) */
    seq_t res = l / (seq_t)n;
    return res;
}
