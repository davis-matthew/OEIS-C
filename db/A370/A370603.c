/*
 * A370603  a(n) = n! * Sum_{d|n} 1/((d-1)! * (n/d)!^(d-1)).
 * https://oeis.org/A370603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} 1/((d-1)! * (n/d)!^(d-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition directly.  No memoisation other than a
   simple factorial cache.  Works for any size of seq_t. */
#include <stdlib.h>                /* malloc, realloc, free */
#include <stdbool.h>               /* bool type */
#include "../utils/pow_seq.h"      /* pow_seq(base, exp) */

seq_t a(idx_t n)
{
    /* factorial cache: fact[i] = i!  for 0 <= i <= max_seen */
    static seq_t *fact = NULL;
    static idx_t fact_max = 0;

    /* Grow cache if needed */
    if (n > fact_max) {
        /* Allocate or reallocate */
        seq_t *new_fact = (seq_t *)realloc(fact, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_fact) {
            /* On allocation failure return 0 (undefined sequence value) */
            return (seq_t)0;
        }
        fact = new_fact;
        if (fact_max == 0) {
            /* initialise 0! = 1 */
            fact[0] = (seq_t)1;
            fact_max = 0;
        }
        /* compute missing factorials */
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }

    seq_t sum = (seq_t)0;
    seq_t fact_n = fact[n];

    /* Enumerate divisors of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* first divisor d = i */
        idx_t d = i;
        idx_t q = n / d;
        seq_t denom = fact[d - 1];
        denom = denom * pow_seq(fact[q], (int)(d - 1));
        sum += fact_n / denom;

        /* second divisor d = n/i, skip if equal to first */
        idx_t d2 = n / i;
        if (d2 == d)
            continue;
        idx_t q2 = n / d2;
        seq_t denom2 = fact[d2 - 1];
        denom2 = denom2 * pow_seq(fact[q2], (int)(d2 - 1));
        sum += fact_n / denom2;
    }

    return sum;
}
