/*
 * A286359  Compound filter: a(n) = P(sigma(n), sigma(2n)), where P(n,k) is
 *          sequence A000027 used as a pairing function, and sigma is the sum
 *          of divisors (A000203).
 * https://oeis.org/A286359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((σ(n)+σ(2n))^2) - σ(n) - 3σ(2n))
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 8399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t s1 = sigma_of(n);
    seq_t s2 = sigma_of(n * 2);
    seq_t sum = s1 + s2;
    seq_t sum_sq = sum * sum;
    seq_t num = (seq_t)2 + sum_sq - s1 - (seq_t)3 * s2;
    return num / (seq_t)2;
}
