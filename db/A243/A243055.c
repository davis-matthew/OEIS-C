/*
 * A243055  Difference between the indices of the smallest and the largest
 *          prime dividing n: If n = p_i * ... * p_k, where p_i <= ... <= p_k,
 *          where p_h = A000040(h), then a(n) = (k-i), a(1) = 0 by convention.
 * https://oeis.org/A243055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between indices of smallest and largest prime
 *            factors of n. a(n) = A061395(n) - A055396(n) where A061395(n) =
 *            index of the largest prime dividing n, A055396(n) = index of the
 *            smallest prime dividing n
 * Range:     u8 n <= 3241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, gpf, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/lpf.h"       /* lpf(uint64_t n) -> smallest prime factor */
#include "../utils/gpf.h"       /* gpf(uint64_t n) -> greatest prime factor */
#include "../utils/compute_pi.h"/* compute_pi(idx_t x) -> number of primes <= x */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 by convention */

    /* Smallest and largest prime factors (values) */
    uint64_t sp = lpf((uint64_t)n);
    uint64_t gp = gpf((uint64_t)n);

    /* If n is prime the difference is zero */
    if (sp == gp) return (seq_t)0;

    /* Convert prime values to their indices via the prime‑counting function */
    seq_t i = compute_pi((idx_t)sp);    /* index of smallest prime factor */
    seq_t k = compute_pi((idx_t)gp);    /* index of largest prime factor */

    return k - i;                       /* difference of indices */
}
