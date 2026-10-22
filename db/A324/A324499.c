/*
 * A324499  a(n) = numerator of Sum_{d|n} sigma(d)/tau(d) where sigma(k) = the
 *          sum of the divisors of k (A000203) and tau(k) = the number of the
 *          divisors of k (A000005).
 * https://oeis.org/A324499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} sigma(d)/tau(d) where sigma(k) =
 *            sum of divisors of k and tau(k) = number of divisors of k
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _floor_isqrt_u64, divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* Utility headers ---------------------------------------------------------- */
#include "../utils/_floor_isqrt_u64.h"   /* idx_t _floor_isqrt_u64(idx_t n); */
#include "../utils/divisor_count_u64.h"  /* uint64_t divisor_count_u64(uint64_t n); */
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n); */
#include "../utils/gcd.h"               /* seq_t gcd(seq_t a, seq_t b); */

/* helper to add the contribution of a single divisor d to the running sum */
static inline void add_divisor(idx_t d, seq_t *sum_num, seq_t *sum_den)
{
    seq_t sigma_d = sigma_of(d);
    seq_t tau_d   = (seq_t)divisor_count_u64(d);   /* tau(d) >= 1 */

    /* new_num = (*sum_num) * tau_d + sigma_d * (*sum_den) */
    seq_t new_num = (*sum_num) * tau_d + sigma_d * (*sum_den);

    /* new_den = (*sum_den) * tau_d */
    seq_t new_den = (*sum_den) * tau_d;

    /* reduce fraction */
    seq_t g = gcd(new_num, new_den);
    *sum_num = new_num / g;
    *sum_den = new_den / g;
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t sum_num = 0;   /* numerator of current sum   */
    seq_t sum_den = 1;   /* denominator of current sum */

    /* iterate over all divisors of n */
    idx_t root = _floor_isqrt_u64(n);
    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            add_divisor(d1, &sum_num, &sum_den);
            if (d2 != d1) {
                add_divisor(d2, &sum_num, &sum_den);
            }
        }
    }

    return sum_num;  /* numerator of the fully reduced sum */
}
