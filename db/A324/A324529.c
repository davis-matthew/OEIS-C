/*
 * A324529  a(n) = lcm(sigma(n), pod(n)) where sigma(k) = the sum of divisors
 *          of k (A000203) and pod(n) = the product of divisors of k
 *          (A007955).
 * https://oeis.org/A324529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(σ(n), pod(n)) where σ(n) is the sum of divisors
 *            (A000203) and pod(n) is the product of divisors (A007955)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, divisor_count_u64, floor_sqrt, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* σ(n) */
    seq_t sigma = sigma_of(n);

    /* pod(n) = n^(d(n)/2) where d(n) is the number of divisors of n */
    uint64_t tau = divisor_count_u64(n);   /* d(n) */
    seq_t pod;
    seq_t nn = (seq_t)n;
    if ((tau & 1ULL) == 0) {                 /* d(n) even */
        pod = big_pow(nn, tau / 2);
    } else {                                 /* d(n) odd → n is a square */
        seq_t sqrt_n = floor_sqrt(nn);
        pod = big_pow(nn, (tau - 1) / 2) * sqrt_n;
    }

    /* lcm(σ, pod) = σ * pod / gcd(σ, pod) */
    seq_t g = gcd(sigma, pod);
    seq_t l = sigma * pod;
    return l / g;
}
