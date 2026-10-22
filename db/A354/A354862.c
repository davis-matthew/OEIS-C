/*
 * A354862  a(n) = n! * Sum_{d|n} (n/d)! / d!.
 * https://oeis.org/A354862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} (n/d)! / d!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* Cached factorials up to fact_upto (inclusive). */
static idx_t fact_upto = 0;
static seq_t *fact_cache = NULL;
static size_t fact_cap = 0;

/* Extend the factorial cache up to n. */
static void
ensure_factorial_cache(idx_t n)
{
    if (n <= fact_upto)
        return;
    /* Allocate or grow the cache to hold indices 0..n. */
    ensure_cache(n, &fact_cache, &fact_cap);
    if (fact_upto == 0) {
        fact_cache[0] = (seq_t)1;
    }
    for (idx_t i = fact_upto + 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_upto = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Offset is 1, but guard against n == 0. */
    if (n == 0)
        return 0;

    /* Build factorials up to n. */
    ensure_factorial_cache(n);
    seq_t fact_n = fact_cache[n];

    /* Sum over divisors of n. */
    seq_t sum = 0;
    idx_t limit = 0;
    /* Simple sqrt loop – safe for n ≤ 10000 and fits in idx_t. */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            /* term for divisor d1 */
            seq_t t1 = fact_n * fact_cache[d2] / fact_cache[d1];
            sum += t1;
            /* if d1 != d2, add term for divisor d2 */
            if (d1 != d2) {
                seq_t t2 = fact_n * fact_cache[d1] / fact_cache[d2];
                sum += t2;
            }
        }
    }
    return sum;
}
