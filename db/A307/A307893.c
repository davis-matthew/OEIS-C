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
 * Helpers:   divisor_count_u64, floor_sqrt, gcd, pow_int, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utility functions */
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n) */
#include "../utils/divisor_count_u64.h" /* size_t divisor_count_u64(uint64_t n) */
#include "../utils/floor_sqrt.h"        /* seq_t floor_sqrt(seq_t x) */
#include "../utils/pow_int.h"           /* seq_t pow_int(seq_t base, idx_t exp) */
#include "../utils/gcd.h"               /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/seq_mul.h"           /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Sum of divisors */
    seq_t sigma = sigma_of(n);

    /* Number of divisors */
    size_t d = divisor_count_u64(n);

    /* Product of divisors = n^{d/2}  (integer) */
    seq_t pod;
    if ((d & 1) == 0) {                     /* d even */
        pod = pow_int((seq_t)n, (idx_t)(d / 2));
    } else {                                 /* d odd – n is a perfect square */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        pod = pow_int(sqrt_n, (idx_t)d);     /* sqrt_n^{d} = n^{d/2} */
    }

    /* Least common multiple of sigma and pod */
    seq_t g = gcd(sigma, pod);
    seq_t l = seq_mul(sigma, pod);
    l = l / g;                               /* lcm(sigma, pod) */

    /* Divide by n to obtain a(n) */
    seq_t res = l / (seq_t)n;
    return res;
}
