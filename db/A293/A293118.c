/*
 * A293118  E.g.f.: exp(x^3/(x-1)).
 * https://oeis.org/A293118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * sum_{k=1..⌊n/3⌋} (-1)^k * C(n-2k-1, k-1) / k!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293118 (OEIS) defined by the EGF exp(x^3/(x-1)) */

#include <stdlib.h>

/* internal factorial cache */
static seq_t *fact = NULL;               /* array of factorials */
static idx_t fact_alloc = 0;             /* allocated size */
static idx_t fact_computed = 0;          /* number of values initialized */

/* Compute and return a(n) */
seq_t a(idx_t n)
{
    /* handle the initial offset */
    if (n == 0) return (seq_t)1;
    if (n < 3)  return (seq_t)0;

    /* ensure factorial table contains n! and k! for all needed k */
    if (n >= fact_computed) {
        size_t needed = (size_t)n + 1;   /* indices 0..n inclusive */
        if (needed > fact_alloc) {
            size_t newsize = fact_alloc ? fact_alloc : 1;
            while (newsize < needed) newsize <<= 1;
            fact = (seq_t *)realloc(fact, newsize * sizeof(seq_t));
            fact_alloc = newsize;
        }
        if (fact_computed == 0) {
            fact[0] = (seq_t)1;
            fact_computed = 1;
        }
        for (idx_t i = fact_computed; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_computed = n + 1;
    }

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;  /* floor(n/3) */
    for (idx_t k = 1; k <= maxk; ++k) {
        /* compute C(n-2k-1, k-1) */
        idx_t D = n - 2 * k - 1;
        idx_t K = k - 1;
        seq_t bin = (seq_t)1;
        for (idx_t i = 1; i <= K; ++i) {
            seq_t num = (seq_t)(D - K + i);
            bin = bin * num / (seq_t)i;
        }

        /* term = (n! / k!) * binomial(D, K) with sign */
        seq_t term = (fact[n] / fact[k]) * bin;
        if (k & 1) /* k odd, sign negative */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
