/*
 * A330718  a(n) = numerator(Sum_{k=1..n} (2^k-2)/k).
 * https://oeis.org/A330718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(sum_{k=1..n} (2^k-2)/k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_uint, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>

/* Utility functions */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/seq_gcd.h"   /* seq_t seq_gcd(seq_t a, seq_t b) */

/* ------------  Implementation  ------------ */

/* Since the sequence is needed up to about 10 000, a static cache of that
 * size is sufficient for the typical test cases.  The cache stores the
 * numerator of the partial sum for each n.  The denominator and the
 * current reduced fraction are kept in two static variables.
 */
#ifndef MAX_CACHESIZE
#define MAX_CACHESIZE 20000
#endif

static seq_t cache[MAX_CACHESIZE];   /* cache[n-1] holds a(n) for 1 ≤ n ≤ MAX_CACHESIZE */
static idx_t cache_n = 0;            /* number of entries already computed */
static seq_t sum_num = 0;            /* numerator of the current reduced sum   */
static seq_t sum_den = 1;            /* denominator of the current reduced sum */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, but return 0 to keep the function defined */
        return 0;

    /* Require that n does not exceed the static cache; the tests use n≈10000 */
    if (n > MAX_CACHESIZE) {
        /* If a larger n is requested, compute it on the fly without caching.
         * This branch will not be hit for the intended problem size.
         */
        seq_t local_num = 0;
        seq_t local_den = 1;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term_num = pow_uint(2, k) - 2;
            seq_t term_den = (seq_t)k;

            if (local_num == 0) {
                seq_t g = seq_gcd(term_num, term_den);
                local_num = term_num / g;
                local_den = term_den / g;
            } else {
                seq_t g1 = seq_gcd(local_den, term_den);
                seq_t denom_lcm = (local_den / g1) * term_den;
                seq_t new_num = local_num * (term_den / g1) + term_num * (local_den / g1);
                seq_t g2 = seq_gcd(new_num, denom_lcm);
                local_num = new_num / g2;
                local_den = denom_lcm / g2;
            }
        }
        return local_num;
    }

    /* Extend the cache up to n if necessary */
    if (n > cache_n) {
        for (idx_t i = cache_n; i < n; ++i) {
            idx_t k = i + 1;                    /* k runs from 1 upward */

            seq_t term_num = pow_uint(2, k) - 2;
            seq_t term_den = (seq_t)k;

            if (sum_num == 0) {                 /* first term specific handling */
                seq_t g = seq_gcd(term_num, term_den);
                sum_num = term_num / g;
                sum_den = term_den / g;
            } else {                            /* add the new term to the sum  */
                seq_t g1 = seq_gcd(sum_den, term_den);
                seq_t denom_lcm = (sum_den / g1) * term_den;
                seq_t new_num = sum_num * (term_den / g1)
                              + term_num * (sum_den / g1);
                seq_t g2 = seq_gcd(new_num, denom_lcm);
                sum_num = new_num / g2;
                sum_den = denom_lcm / g2;
            }

            cache[i] = sum_num;                  /* store the reduced numerator */
        }
        cache_n = n;
    }

    return cache[n - 1];
}
