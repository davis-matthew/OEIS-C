/*
 * A356009  a(n) = n! * Sum_{k=1..n} Sum_{d|k} 1/(d * (k/d)!).
 * https://oeis.org/A356009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} Sum_{d|k} 1/(d * (k/d)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A356009 (offset 1) */

#include <stdlib.h>

/* Cache of factorials: fact[i] = i!   (i <= fact_len-1) */
static seq_t *fact = NULL;
static idx_t fact_len = 0;

/* Ensure that fact[0..n] are computed (inclusive).  Uses modular arithmetic
   for seq_t, so works for arbitrary-precision types as well. */
static void ensure_fact(idx_t n)
{
    if (n < fact_len)
        return;

    idx_t new_len = n + 1;                    /* need indices 0..n */
    seq_t *tmp = (seq_t *)realloc(fact, new_len * sizeof(seq_t));
    if (!tmp)
        return;                               /* allocation failed – caller will crash */

    fact = tmp;

    /* If this is the first allocation, set fact[0] = 1 */
    if (fact_len == 0) {
        fact[0] = (seq_t)1;
        fact_len = 1;
    }

    /* Compute missing factorials */
    for (idx_t i = fact_len; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;      /* exact division in seq_t */
    }
    fact_len = new_len;
}

/* Main sequence function.  For n >= 1 returns the nth term; behaviour
   for n == 0 is irrelevant. */
seq_t a(idx_t n)
{
    if (n == 0)            /* below offset – undefined; return 0 */
        return 0;

    ensure_fact(n);
    seq_t fact_n = fact[n];
    seq_t total = (seq_t)0;

    /* Loop over k = 1..n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Enumerate all divisors d of k by checking up to sqrt(k) */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d != 0)
                continue;

            /* First divisor d */
            idx_t m1 = k / d;                     /* m1 = k/d */
            seq_t denom1 = fact[m1] * (seq_t)d;   /* d * (k/d)! */
            seq_t term1 = fact_n / denom1;        /* exact division */
            total += term1;

            /* Second divisor k/d (if distinct) */
            idx_t d2 = k / d;                     /* d2 = k/d */
            if (d2 != d) {
                idx_t m2 = k / d2;                /* m2 = d */
                seq_t denom2 = fact[m2] * (seq_t)d2;
                seq_t term2 = fact_n / denom2;
                total += term2;
            }
        }
    }

    return total;
}
