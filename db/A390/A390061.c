/*
 * A390061  E.g.f. A(x) satisfies A(x) = exp( x * (1+x^2)^3 * A(x)^2 ).
 * https://oeis.org/A390061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} n!/(n-2*k)! * * *
 *            (2*(n-2*k)+1)^(n-2*k-1) * binomial(3*(n-2*k),k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_seq_array, big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A390061: a(0), a(1), a(2), … */
#include <stdint.h>
#include <stdlib.h>

#include "../utils/binomial.h"      /* binomial(idx_t n, idx_t k)   */
#include "../utils/big_pow.h"       /* big_pow(seq_t base, seq_t exp) */
#include "../utils/alloc_seq_array.h" /* alloc_seq_array(size_t n) */

static seq_t *fact_table = NULL;  /* factorial cache: fact_table[i] = i! */
static idx_t fact_len = 0;        /* highest n for which fact_table[n] is valid */

/* Ensure factorial table is filled up to n. */
static void
ensure_fact(idx_t n)
{
    if (n < fact_len) return;  /* already computed enough */

    /* Grow table if needed. */
    if (fact_table == NULL) {
        fact_table = alloc_seq_array(n + 1);
        fact_len    = 0;
    } else if (fact_len < n) {
        /* realloc using the provided allocator. */
        fact_table = (seq_t *)realloc(fact_table, (n + 1) * sizeof(seq_t));
        if (!fact_table) abort();   /* allocation failure */
    }

    /* Compute factorials from the next missing entry up to n. */
    if (fact_len == 0) {
        fact_table[0] = (seq_t)1;
    }
    for (idx_t i = fact_len + 1; i <= n; ++i) {
        fact_table[i] = fact_table[i - 1] * (seq_t)i;
    }
    fact_len = n;
}

seq_t a(idx_t n)
{
    ensure_fact(n);           /* we need factorials up to n for the ratio n!/m! */

    seq_t sum = 0;

    /* Walk through k = 0 .. floor(n/2) and accumulate the term. */
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - 2 * k;  /* m = n - 2k   (the exponent of the power term) */

        /* Compute n! / m! efficiently using a running ratio. */
        /* ratio = product_{i=m+1}^{n} i.  For k=0, ratio=1. */
        static seq_t ratio = 1;  /* static so it persists across calls,
                                    we reset it for each new n */
        if (k == 0) {
            ratio = 1;
        } else {
            /* previous m' = n - 2*(k-1) = m + 2 */
            ratio *= (seq_t)(m + 2) * (seq_t)(m + 1);
        }

        /* Power term (2m+1)^(m-1); define it as 1 when m==0 to avoid negative
           exponent pitfalls. */
        seq_t power;
        if (m == 0) {
            power = (seq_t)1;
        } else {
            power = big_pow((seq_t)(2 * m + 1), (seq_t)(m - 1));
        }

        /* Binomial coefficient; avoid calling it when m==0 (k = n/2). */
        seq_t term;
        if (m == 0) {
            /* In this case binomial(0, k) is zero unless k==0.
               For n=0 we already handled the term above. */
            if (k == 0) {
                term = (seq_t)1;   /* term = n! / 0! * 1 * 1 = 1 for n=0 */
            } else {
                term = (seq_t)0;
            }
        } else {
            seq_t bin = binomial(3 * m, k);
            term = ratio * power * bin;
        }

        sum += term;
    }

    return sum;
}
