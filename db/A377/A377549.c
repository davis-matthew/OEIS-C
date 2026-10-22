/*
 * A377549  E.g.f. satisfies A(x) = 1 + x*A(x)^5*exp(x*A(x)^2).
 * https://oeis.org/A377549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(2*n+3*k+1,k) /
 *            ((2*n+3*k+1)*(n-k)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

static seq_t *fact_arr = NULL;      /* factorials */
static size_t fact_cap = 0;

/* ensure fact_arr has capacity for indices 0..n */
static void ensure_fact_cap(idx_t n)
{
    if (fact_cap <= (size_t)n) {
        size_t new_cap = fact_cap ? fact_cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        fact_arr = (seq_t*)realloc(fact_arr, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_fact_cap(n);

    /* build factorial table up to n if needed */
    if (fact_cap == 0 || fact_arr[n] == (seq_t)0) {
        fact_arr[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) {
            fact_arr[i] = fact_arr[i-1] * (seq_t)i;
        }
    }

    seq_t fact_n = fact_arr[n];
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {

        /* falling factorial: n! / (n-k)! */
        seq_t falling = fact_n / fact_arr[n - k];

        /* k^(n-k) */
        seq_t pow_term = big_pow((seq_t)k, (seq_t)(n - k));

        /* denominator part */
        idx_t denom_idx = 2*n + 3*k + 1;
        seq_t denom = (seq_t)denom_idx;

        /* binomial part */
        seq_t binom_val = binomial(denom_idx, k);

        /* term = falling * pow_term * binom_val / denom */
        seq_t term = falling * pow_term;
        term = term * binom_val;
        term = term / denom;

        sum += term;
    }

    return sum;
}
