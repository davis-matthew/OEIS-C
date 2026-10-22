/*
 * A382483  a(n) = smallest number k such that at least one of sigma(n) - k
 *          and sigma(n) + k is a perfect number.
 * https://oeis.org/A382483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) = minimal k such that sigma(n) - k or sigma(n) + k is a perfect number
   (OEIS A382483).  The implementation uses trial division for sigma(n) and
   compares the result with the first eight known even perfect numbers.  No
   external libraries are required. This function works for any seq_t/idx_t
   types used by the build. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* The first eight even perfect numbers (all < 2^64).  For arbitrary precision
   seq_t, these literals are converted automatically. */
static const seq_t perfect_numbers[] = {
    (seq_t)6ULL,
    (seq_t)28ULL,
    (seq_t)496ULL,
    (seq_t)8128ULL,
    (seq_t)33550336ULL,
    (seq_t)8589869056ULL,          /* 2^33 * 521          */
    (seq_t)137438691328ULL,        /* 2^35 * 1099511627775*/
    (seq_t)2305843008139952128ULL  /* 2^30 * (2^31-1)     */
};
static const size_t perfect_count = sizeof(perfect_numbers) / sizeof(perfect_numbers[0]);

/* Sum of divisors of n (including n).  For n up to about 10⁵ this runs <1 ms. */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = (seq_t)1;
    idx_t m = n;

    for (idx_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            /* Compute (1 + d + d² + … + dᵏ) where k = exponent of d in m. */
            seq_t term_power = (seq_t)1;
            seq_t term_sum   = (seq_t)1;
            while (m % d == 0) {
                m /= d;
                term_power *= (seq_t)d;
                term_sum   += term_power;
            }
            sum *= term_sum;
        }
    }

    if (m > 1)               /* remaining prime factor */
        sum *= (seq_t)(1 + m);

    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    /* Initialise min_k with the distance to the first perfect number
       to avoid problems with signed seq_t being negative. */
    seq_t min_k = (sigma > perfect_numbers[0]) ?
                   (sigma - perfect_numbers[0]) :
                   (perfect_numbers[0] - sigma);

    for (size_t i = 1; i < perfect_count; ++i) {
        seq_t p = perfect_numbers[i];
        seq_t diff = (sigma > p) ? (sigma - p) : (p - sigma);
        if (diff < min_k) {
            min_k = diff;
            if (min_k == (seq_t)0) break;   /* exact match */
        }
    }

    return min_k;
}
