/*
 * A302582  a(n) = n! * [x^n] log(1 + x)/(1 - x)^n.
 * https://oeis.org/A302582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} (-1)^(k+1) * binomial(2*n-k-1, n-k) / k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"

/* Static cache for factorials */
static seq_t *fact_cache = NULL;
static size_t fact_cache_cap = 0;
static size_t fact_cache_count = 0;

static void ensure_factorial(idx_t n)
{
    if (fact_cache_cap < (size_t)(n + 1)) {
        size_t new_cap = fact_cache_cap ? (fact_cache_cap << 1) : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        fact_cache = tmp;
        fact_cache_cap = new_cap;
    }

    if (fact_cache_count <= (size_t)n) {
        for (size_t i = fact_cache_count; i <= (size_t)n; ++i) {
            if (i == 0)
                fact_cache[i] = (seq_t)1;
            else
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_count = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_factorial(n);
    seq_t fact_n = fact_cache[n];

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* Binomial B_1 = binomial(2n-2, n-1) */
    seq_t B = binom(2 * n - 2, n - 1);

    for (idx_t k = 1; k <= n; ++k) {
        /* term = B * (fact_n / k) */
        seq_t term = B * (fact_n / (seq_t)k);
        if (k & 1u)
            sum_pos += term;
        else
            sum_neg += term;

        if (k < n) {
            /* Update B_{k+1} = B * (n - k) / (2n - k - 1) */
            seq_t num = B * (seq_t)(n - k);
            B = num / (seq_t)(2 * n - k - 1u);
        }
    }
    return sum_pos - sum_neg;
}
